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

    template <typename X>
    using cell_unordered = std::unordered_map<std::string, X>;

    using cell<T, std::string, sir, vicinity>::simulation_clock;
    using cell<T, std::string, sir, vicinity>::state;
    using cell<T, std::string, sir, vicinity>::neighbors;

    using config_type = simulation_configuration;

    using phase_rates = std::vector<            // The age sub_division
                        std::vector<double>>;   // The stage of infection

    phase_rates virulence_rates;
    phase_rates recovery_rates;
    phase_rates mobility_rates;

    double disobedient;  // percentage of population that do not follow the quarantine restrictions

    // To make the parameters of the correction_factors variable more obvious
    using infection_threshold = float;
    using mobility_correction_factor = float;

    std::map<infection_threshold, mobility_correction_factor> correction_factors;

    int precDivider;

    zhong_cell() : cell<T, std::string, sir, vicinity>() {}

    zhong_cell(std::string const &cell_id, cell_unordered<vicinity> const &neighborhood, sir initial_state,
            std::string const &delay_id, simulation_configuration config) :
    cell<T, std::string, sir, vicinity>(cell_id, neighborhood, initial_state, delay_id) {

        virulence_rates = std::move(config.virulence_rates);
        recovery_rates = std::move(config.recovery_rates);
        mobility_rates = std::move(config.mobility_rates);

        correction_factors = std::move(config.correction_factors);
        disobedient = config.disobedient;

        precDivider = config.precision;

        assert(virulence_rates.size() == recovery_rates.size() && virulence_rates.size() == mobility_rates.size() &&
               "\n\nThere must be an equal number of age segments between all configuration rates.\n\n");
    }

    // user must define this function. It returns the next cell state and its corresponding timeout
    sir local_computation() const override {

        sir res = state.current_state;

        // Whenever referring to a "population", it is meant the current age group's population.
        for(int age_segment_index = 0; age_segment_index < res.get_num_age_segments(); ++age_segment_index) {

            double new_i = std::round(new_infections(age_segment_index) * precDivider) / precDivider;

            // Of the population that is on the last day of the infection, they are now considered recovered.
            double new_r = res.infected[age_segment_index].back();

            double new_s = 1;

            // Equation 6e
            for (int i = 0; i < res.get_num_recovered_phases() - 1; ++i)
            {
                // Calculate all of the new recovered- for every day that a population is infected, some recover.
                new_r += std::round(res.infected[age_segment_index][i] * recovery_rates[age_segment_index][i] * precDivider) / precDivider;
            }

            // Equation 6d

            for (int i = res.get_num_infected_phases() - 1; i > 0; --i)
            {
                // *** Calculate proportion of infected on a given day of the infection ***

                // The previous day of infection
                double curr_inf = res.infected[age_segment_index][i - 1];

                // A proportion of the previous day's infection recovers, leading to a new proportion
                // of the population that is currently infected at the current day of infection
                curr_inf *= 1 - recovery_rates[age_segment_index][i - 1];

                curr_inf = std::round(curr_inf * precDivider) / precDivider;

                // The amount of susceptible does not include the infected population
                new_s -= curr_inf;

                res.infected[age_segment_index][i] = curr_inf;
            }

            // The people on the first day of infection are equal to the number of infections from the susceptible population
            res.infected[age_segment_index][0] = new_i;

            // The susceptible population does those that just became infected
            new_s -= new_i;

            // Equation 6a
            for (int i = res.get_num_recovered_phases() - 1; i > 0; --i)
            {
                // Each day of the recovered is the value of the previous day. The population on the last day is
                // now susceptible; this is implicitly done already as the susceptible value was set to 1.0 and the
                // population on the last day of recovery is never subtracted from the susceptible value.
                res.recovered[age_segment_index][i] = res.recovered[age_segment_index][i - 1];
                new_s -= res.recovered[age_segment_index][i];
            }

            // The people on the first day of recovery are those that were on the last stage of infection
            // in the previous time step plus those that recovered early during an infection stage.
            res.recovered[age_segment_index][0] = new_r;

            // The susceptible population does not include the recovered population
            new_s -= new_r;

            if (new_s > -0.001 && new_s < 0) new_s = 0;  // double precision issues

            assert(new_s >= 0);
            res.susceptible[age_segment_index] = new_s;
        }

        return res;
    }
    // It returns the delay to communicate cell's new state.
    T output_delay(sir const &cell_state) const override {
        return 1;
    }

    double new_infections(unsigned int age_segment_index) const {
        double inf = 0;
        sir cstate = state.current_state;

        // disobedient people have a correction factor of 1. The rest of the population -> whatever in the configuration
        float correction = disobedient + (1 - disobedient) * movement_correction_factor();

        // external infected
        for(auto neighbor: neighbors) {
            sir nstate = state.neighbors_state.at(neighbor);
            vicinity v = state.neighbors_vicinity.at(neighbor);

            float total_neighbouring_infections = 0.0f;

            for(const auto &infected : nstate.infected) {
                total_neighbouring_infections += std::accumulate(infected.begin(), infected.end(), 0.0f);
            }

            for (int i = 0; i < nstate.get_num_infected_phases(); ++i) {

                inf += v.correlation * mobility_rates[age_segment_index][i] * // variable Cij
                       virulence_rates[age_segment_index][i] * // variable lambda
                       cstate.susceptible[age_segment_index] * total_neighbouring_infections * // variables Si and Ij, respectively
                       correction;  // New infections may be slightly fewer if there are mobility restrictions

            }
        }

        return std::min(cstate.susceptible[age_segment_index], inf);
    }

    float movement_correction_factor() const {

        sir res = state.current_state;

        float total_infections = 0.0f;

        // Get the the portion of the population that is infected, which is the sum of the infected population of all of
        // the groups for the current cell.
        for(auto &infected : res.infected) {
            total_infections += std::accumulate(infected.begin(), infected.end(), 0.0f);
        }

        float correction = 1.0f;
        for (auto const &pair: correction_factors) {
            if (total_infections >= pair.first) {
                correction = pair.second;
            }
            else {
                break;
            }
        }

        return correction;
    }
};

#endif //PANDEMIC_HOYA_2002_ZHONG_CELL_HPP
