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
#include "simulation_configuration.hpp"

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

    using config_type = simulation_configuration;

    using phase_rates =
                        std::vector<            // The age sub_division
                        std::vector<double>>;   // The stage of infection

    phase_rates virulence_rates;
    phase_rates recovery_rates;
    phase_rates mobility_rates;
    std::vector<float> mobility_correction_factor;

    double disobedient;
    int precDivider;

    zhong_cell() : cell<T, std::string, sir, vicinity>() {}

    zhong_cell(std::string const &cell_id, cell_unordered<vicinity> const &neighborhood, sir initial_state, std::string const &delay_id, simulation_configuration config) :
    cell<T, std::string, sir, vicinity>(cell_id, neighborhood, initial_state, delay_id) {

        virulence_rates = std::move(config.virulence_rates);
        recovery_rates = std::move(config.recovery_rates);
        mobility_rates = std::move(config.mobility_rates);
        mobility_correction_factor = std::move(config.mobility_correction_factor);

        disobedient = config.disobedient;
        precDivider = config.precision;

        assert(virulence_rates.size() == recovery_rates.size() &&
                       virulence_rates.size() == mobility_rates.size() &&
               "\n\nThere must be an equal number of age segments between all configuration rates.\n\n");
    }

    // user must define this function. It returns the next cell state and its corresponding timeout
    sir local_computation() const override {

        sir res = state.current_state;

        double new_i = std::round(get_phase_penalty(res.phase) * new_infections() * precDivider) / precDivider;

        // Of the population that is on the last day of the infection, they are now considered recovered.
        double new_r = res.infected.back();

        double new_s = 1;

        // Equation 6e
        for(int age_sub_division = 0; age_sub_division < res.age_divided_populations.size(); ++age_sub_division) {
            for (int i = 0; i < res.get_num_infected() - 1; ++i) {
                // Calculate all of the new recovered- for every day that a population is infected, some recover.

                new_r += std::round(res.age_divided_populations[age_sub_division] * res.infected[i] * recovery_rates[age_sub_division][i] * precDivider) / precDivider;;
            }
        }

        // Equation 6b is done through several steps, every time new_s is subtracted from


        // Equation 6d

        // The infections have to be updated only after all age group infection calculations are done; otherwise iterations
        // after the first iteration will use invalid infected values. The first infection stage will have the value of new_i,
        // so one less infection states needs to be kept tracked of.
        std::vector<double> new_infections(res.get_num_infected() - 1, 0.0f);

        for(int age_sub_division = 0; age_sub_division < res.age_divided_populations.size(); ++age_sub_division) {
            for (int i = res.get_num_infected() - 1; i > 0; --i)
            {
                // *** Calculate proportion of infected on a given day of the infection ***

                // The previous day of infection
                double curr_inf = res.infected[i - 1] * res.age_divided_populations[age_sub_division];

                // A proportion of the previous day's infection recovers, leading to a new proportion
                // of the population that is currently infected at the current day of infection
                curr_inf *= 1 - recovery_rates[age_sub_division][i - 1];

                curr_inf = std::round(curr_inf * precDivider) / precDivider;

                // The amount of susceptible does not include the amount the infected proportion of the population
                new_s -= curr_inf;

                // Update the new infections value for the current infection stage, that the current age group contributed to
                new_infections[i - 1] += curr_inf;
            }
        }

        // Now update all of the infection values for all of the infection stage
        std::copy(new_infections.begin(), new_infections.end(), res.infected.begin() + 1);

        // The people on the first day of infection are equal to the number of infections from the susceptible population
        res.infected[0] = new_i;

        // The susceptible population does not include infected individuals
        new_s -= new_i;

        // Equation 6a
        for (int i = res.get_num_recovered() - 1; i > 0; --i)
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

        if(new_s > -0.001 && new_s < 0) new_s = 0;  // double precision issues

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
        double sum = init.susceptible;
        assert(init.susceptible >= 0 && init.susceptible <= 1);
        for(int i=0; i < init.get_num_infected(); i++) {
            assert(init.infected[i] >= 0 && init.infected[i] <= 1);
            sum += init.infected[i];
        }
        for(int i=0; i < init.get_num_recovered(); i++) {
            assert(init.recovered[i] >= 0 && init.recovered[i] <= 1);
            sum += init.recovered[i];
        }
        assert(sum == 1);
    }

    double new_infections() const {
        double inf = 0;
        sir cstate = state.current_state;

        // external infected
        for(auto neighbor: neighbors) {
            sir nstate = state.neighbors_state.at(neighbor);
            vicinity v = state.neighbors_vicinity.at(neighbor);

            for (int age_sub_division = 0; age_sub_division < nstate.age_divided_populations.size(); ++age_sub_division) {
                for (int i = 0; i < nstate.get_num_infected(); ++i) {

                    inf += v.correlation * mobility_rates[age_sub_division][i] * // variable Cij
                           virulence_rates[age_sub_division][i] * // variable lambda
                           cstate.susceptible * nstate.infected[i] * // variables Si and Ij, respectively
                           cstate.age_divided_populations[age_sub_division] * // The amount of new infections from the current sub_population
                           mobility_correction_factor[cstate.phase];

                }
            }
        }

        return std::min(cstate.susceptible, inf);
    }

    double get_phase_penalty(unsigned int phase) const {
        // All rates vector have the same number of phases, so it doesn't matter which one is used here
        assert(0 <= phase && phase < virulence_rates.size());
        return 1;
    }

    unsigned int next_phase(unsigned int phase) const {
        return 0;
    }
};

#endif //PANDEMIC_HOYA_2002_ZHONG_CELL_HPP