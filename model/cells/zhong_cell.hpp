//
// Created by binybrion on 6/29/20.
//

#ifndef PANDEMIC_HOYA_2002_ZHONG_CELL_HPP
#define PANDEMIC_HOYA_2002_ZHONG_CELL_HPP

#include <cmath>
#include <iostream>
#include <vector>
#include <cadmium/celldevs/cell/cell.hpp>
#include <iomanip>
#include "vicinity.hpp"
#include "sir.hpp"
#include "rates_configuration.h"

using namespace std;
using namespace cadmium::celldevs;

template <typename T>
class zhong_cell : public cell<T, std::string, sir, vicinity> {
public:

    template<typename X>
    using cell_unordered = std::unordered_map<std::string, X>;

    using cell<T, std::string, sir, vicinity>::simulation_clock;
    using cell<T, std::string, sir, vicinity>::state;
    using cell<T, std::string, sir, vicinity>::neighbors;

    using config_type = rates_configurations;

    using PhaseRates = std::vector<float>;

    std::vector<PhaseRates> virulency_rates;
    std::vector<PhaseRates> recovery_rates;
    std::vector<PhaseRates> mobility_rates;

    float disobedient = 0.0;
    int precDivider = 1000;

    zhong_cell() : cell<T, std::string, sir, vicinity>() {}

    zhong_cell(std::string const &cell_id, cell_unordered<vicinity> const &neighborhood, sir initial_state, std::string const &delay_id, rates_configurations config) :
    cell<T, std::string, sir, vicinity>(cell_id, neighborhood, initial_state, delay_id) {

        virulency_rates = std::move(config.virulency_rates);
        recovery_rates = std::move(config.recovery_rates);
        mobility_rates = std::move(config.mobility_rates);

        assert(virulency_rates.size() == recovery_rates.size() &&
               virulency_rates.size() == mobility_rates.size() &&
               recovery_rates.size() == mobility_rates.size() &&
               "\n\nThere must be an equal number of phases between all configuration rates.\n\n");
    }

    // user must define this function. It returns the next cell state and its corresponding timeout
    sir local_computation() const override {

        sir res = state.current_state;

        float new_i = std::round(get_phase_penalty(res.phase) * new_infections() * precDivider) / precDivider;

        // Of the population that is on the last day of the infection, they are now considered recovered.
        float new_r = res.infected[res.num_inf-1];

        float new_s = 1;

        // Equation 6e
        for (int i = 0; i < res.num_inf - 1; ++i) {
            // Calculate all of the new recovered- for every day that a population is infected, some recover.
            new_r += std::round(res.infected[i] * recovery_rates[res.phase][i] * precDivider) / precDivider;
        }

        // Equation 6b is done through several steps, every time new_s is subtracted from

        // Equation 6d
        for (int i = res.num_inf - 1; i > 0; --i) {
            // *** Calculate proportion of infected on a given day of the infection ***

            // The previous day of infection
            float curr_inf = res.infected[i-1];

            // A proportion of the previous day's infection recovers, leading to a new proportion
            // of the population that is currently infected at the current day of infection
            curr_inf *= 1 - recovery_rates[res.phase][i-1];

            res.infected[i] = std::round(curr_inf * precDivider) / precDivider;

            // The amount of susceptible does not include the amount the infected proportion of the population
            new_s -= res.infected[i];
        }

        // The people on the first day of infection are equal to the number of infections from the susceptible population
        res.infected[0] = new_i;

        // The susceptible population does not include infected individuals
        new_s -= new_i;

        // Equation 6a
        for (int i = res.num_rec - 1; i > 0; --i)
        {
            // Each day of the recovered is the value of the previous day. The population on the last day is
            // now susceptible; this is implicitly done already as the susceptible value was set to 1.0 and the
            // population on the last day of recovery is never subtracted from the susceptible value.
            res.recovered[i] = res.recovered[i-1];
            new_s -= res.recovered[i];
        }

        // The people on the first day of recovery are those that were on the last stage of infection
        // in the previous time step.
        res.recovered[0] = new_r;

        // The susceptible population does not include the recovered population
        new_s -= new_r;

        if(new_s > -0.001 && new_s < 0) new_s = 0;  // float precision issues

        assert(new_s >= 0);
        res.susceptible = new_s;

        res.phase = next_phase(res.phase);

        return res;
    }
    // It returns the delay to communicate cell's new state.
    T output_delay(sir const &cell_state) const override {
        return 1;
    }

    void check_valid_vicinity() {

        sir cstate = state.current_state;

        for(const auto& neighbor : neighbors) {

            sir nstate = state.neighbors_state.at(neighbor);
            vicinity v = state.neighbors_vicinity.at(neighbor);

            assert(v.correlation >= 0 && v.correlation <= 1);
        }
    }

    void check_valid_state() const {
        sir init = state.current_state;
        float sum = init.susceptible;
        assert(init.susceptible >= 0 && init.susceptible <= 1);
        for(int i=0; i < init.num_inf; i++) {
            assert(init.infected[i] >= 0 && init.infected[i] <= 1);
            sum += init.infected[i];
        }
        for(int i=0; i < init.num_rec; i++) {
            assert(init.recovered[i] >= 0 && init.recovered[i] <= 1);
            sum += init.recovered[i];
        }
        assert(sum == 1);
    }

    float new_infections() const {
        float inf = 0;
        sir cstate = state.current_state;

        // internal infected
        for (int i = 0; i < cstate.num_inf; ++i) {
            inf +=   virulency_rates[cstate.phase][i] * // variable lambda
                     cstate.susceptible * cstate.population_density * // variable Rho(i)
                     cstate.susceptible * cstate.infected[i]; // variables Si and Ii, respectively
        }
        inf *= cstate.susceptible;

        // external infected
        for(auto neighbor: neighbors) {
            sir nstate = state.neighbors_state.at(neighbor);
            vicinity v = state.neighbors_vicinity.at(neighbor);

            for (int i = 0; i < nstate.num_inf; ++i) {

                inf +=  v.correlation * mobility_rates[cstate.phase][i] * // variable Cij
                        virulency_rates[cstate.phase][i] * // variable lambda
                        cstate.susceptible * cstate.population_density * // variable Rho(i)
                        cstate.susceptible * nstate.infected[i]; // variables Si and Ij, respectively
            }
        }

        return std::min(cstate.susceptible, inf);
    }

    float get_phase_penalty(unsigned int phase) const {
        // All rates vector have the same number of phases, so it doesn't matter which one is used here
        assert(0 <= phase && phase < virulency_rates.size());
        return 1;
    }

    unsigned int next_phase(unsigned int phase) const {
        return 0;
    }
};

#endif //PANDEMIC_HOYA_2002_ZHONG_CELL_HPP
