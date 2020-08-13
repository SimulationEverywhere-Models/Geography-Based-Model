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
#include "simulation_config.hpp"

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

    using config_type = simulation_config;

    using phase_rates = std::vector<            // The age sub_division
                        std::vector<double>>;   // The stage of infection

    phase_rates virulence_rates;
    phase_rates recovery_rates;
    phase_rates mobility_rates;
    phase_rates fatality_rates;

    double hospital_infected_capacity;
    double over_capacity_fatality_modifier;

    double disobedient;  // percentage of population that do not follow the quarantine restrictions

    // To make the parameters of the correction_factors variable more obvious
    using infection_threshold = float;
    using mobility_correction_factor = std::array<float, 2>; // The first value is the mobility correction factor;
                                                             // The second one is the hysteresis factor.

    std::map<infection_threshold, mobility_correction_factor> correction_factors;

    int precDivider;

    zhong_cell() : cell<T, std::string, sir, vicinity>() {}

    zhong_cell(std::string const &cell_id, cell_unordered<vicinity> const &neighborhood, sir initial_state,
               std::string const &delay_id, simulation_config config) :
    cell<T, std::string, sir, vicinity>(cell_id, neighborhood, initial_state, delay_id) {

        virulence_rates = std::move(config.virulence_rates);
        recovery_rates = std::move(config.recovery_rates);
        mobility_rates = std::move(config.mobility_rates);
        fatality_rates = std::move(config.fatality_rates);

        correction_factors = std::move(config.correction_factors);
        disobedient = config.disobedient;
        hospital_infected_capacity = config.hospital_infected_capacity;
        over_capacity_fatality_modifier = config.over_capacity_fatality_modifier;

        precDivider = config.precision;

        assert(virulence_rates.size() == recovery_rates.size() && virulence_rates.size() == mobility_rates.size() &&
               "\n\nThere must be an equal number of age segments between all configuration rates.\n\n");
    }

    // user must define this function. It returns the next cell state and its corresponding timeout
    sir local_computation() const override {

        sir res = state.current_state;

        // Whenever referring to a "population", it is meant the current age group's population.
        // These calculations are independent of the other age groups, meaning that the proportion that the current
        // age group contributes to the population does not need to be taken into account.
        for(int age_segment_index = 0; age_segment_index < res.get_num_age_segments(); ++age_segment_index) {

            double new_i = std::round(new_infections(age_segment_index) * precDivider) / precDivider;

            // Of the population that is on the last day of the infection, they are now considered recovered.
            std::vector<double> recovered(res.get_num_infected_phases(), 0.0f);
            recovered.back() = res.infected[age_segment_index].back();

            std::vector<double> fatalities(res.get_num_infected_phases(), 0.0f);

            // The susceptible population is smaller due to previous deaths
            double new_s = 1 - res.fatalities[age_segment_index];

            // Note: Remember that these recoveries and fatalities are from the previous simulation cycle. Thus there is an ordering
            // issue- recovery rate and fatality rates specify a percentage of the infected at a certain stage. As a result
            // the code cannot for example, calculate recovery, change the infected stage population, and then calculate
            // fatalities, or vice-versa. This would change the meaning of the input.

            // Equation 6e
            for (int i = 0; i < res.get_num_infected_phases() - 1; ++i)
            {
                // Calculate all of the new recovered- for every day that a population is infected, some recover.
                recovered[i] += std::round(res.infected[age_segment_index][i] * recovery_rates[age_segment_index][i] * precDivider) / precDivider;
            }

            // Calculate all those who have died during an infection stage.
            for(int i = 0; i < res.get_num_infected_phases(); ++i)
            {
                fatalities[i] += std::round(res.infected[age_segment_index][i] * fatality_rates[age_segment_index][i] * precDivider) / precDivider;

                if(res.get_total_infections() > hospital_infected_capacity) {
                    fatalities[i] *= over_capacity_fatality_modifier;

                    // Any stage before last stage of infection
                    if(i != res.get_num_infected_phases() - 1) {
                        // There can't be more fatalities than the number of people who are infected at a stage plus
                        // those who recover at that stage
                        if(fatalities[i] > (res.infected[age_segment_index][i] - recovered[i])) {
                            fatalities[i] = res.infected[age_segment_index][i] - recovered[i];
                        }
                    }
                    // Last stage of infection
                    else {
                        // Since the number of recovered individuals on the first day of recovery was already set to be
                        // the number of people on the last stage of infection, the above if-branch will always set
                        // fatalities on the last stage of infection equal to 0. Thus for the last stage of infection,
                        // fatalities are capped to the number of people who are on the last stage of infected.
                        // The logic of this branch is a result of the note above.
                        if(fatalities.back() > res.infected[age_segment_index].back()) {
                            fatalities[i] = res.infected[age_segment_index].back();
                        }
                    }
                }
            }

            res.fatalities[age_segment_index] += std::accumulate(fatalities.begin(), fatalities.end(), 0.0f);

            // The susceptible population does not include deaths
            new_s -= std::accumulate(fatalities.begin(), fatalities.end(), 0.0f);

            // So far, it was assumed that on the last day of infection, all recovered. But this is not true- have to account
            // for those who died on the last day of infection.
            recovered.back() -= fatalities.back();

            // Equation 6d

            for (int i = res.get_num_infected_phases() - 1; i > 0; --i)
            {
                // *** Calculate proportion of infected on a given day of the infection ***

                // The previous day of infection
                double curr_inf = res.infected[age_segment_index][i - 1];

                // The number of people in a stage of infection moving to the new infection stage do not include those
                // who have died or recovered. Note: A subtraction must be done here as the recovery and mortality rates
                // are given for the total population of an infection stage. Multiplying by (1 - respective rate) here will
                // NOT work as the second multiplication done will effectively be of the infection stage population after
                // the first multiplication, rather than the entire infection state population.
                curr_inf -= recovered[i - 1];
                curr_inf -= fatalities[i - 1];

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

            // The people on the first day of recovery are those that were on the last stage of infection (minus those who died;
            // already accounted for) in the previous time step plus those that recovered early during an infection stage.
            res.recovered[age_segment_index][0] = std::accumulate(recovered.begin(), recovered.end(), 0.0f);

            // The susceptible population does not include the recovered population
            new_s -= std::accumulate(recovered.begin(), recovered.end(), 0.0f);

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

        // external infected
        for(auto neighbor: neighbors) {
            sir nstate = state.neighbors_state.at(neighbor);
            vicinity v = state.neighbors_vicinity.at(neighbor);

            // disobedient people have a correction factor of 1. The rest of the population -> whatever in the configuration
            float correction = disobedient + (1 - disobedient) * movement_correction_factor(v.correction_factors, nstate.get_total_infections(), v.neighbour_hysteresis_factor);

            for (int i = 0; i < nstate.get_num_infected_phases(); ++i) {

                inf += v.correlation * mobility_rates[age_segment_index][i] * // variable Cij
                       virulence_rates[age_segment_index][i] * // variable lambda
                       cstate.susceptible[age_segment_index] * nstate.get_total_infections() * // variables Si and Ij, respectively
                       correction;  // New infections may be slightly fewer if there are mobility restrictions
            }
        }

        return std::min(cstate.susceptible[age_segment_index], inf);
    }

    float movement_correction_factor(const std::map<infection_threshold, mobility_correction_factor> &mobility_correction_factors,
                                     float infectious_population, hysteresis_factor &hysteresisFactor) const {

        // For example, assume a correction factor of "0.4": [0.2, 0.1]. If the infection goes above 0.4, then the
        // correction factor of 0.2 will now be applied to total infection values above 0.3, no longer 0.4 as the
        // hysteresis is in effect.
        if(infectious_population > hysteresisFactor.infections_higher_bound) {
            hysteresisFactor.in_effect = false;
        }

        if(hysteresisFactor.in_effect && infectious_population >= hysteresisFactor.infections_lower_bound) {
            return hysteresisFactor.mobility_correction_factor;
        }

        hysteresisFactor.in_effect = false;

        float correction = 1.0f;
        for (auto const &pair: correction_factors) {
            if (infectious_population >= pair.first) {
                correction = pair.second.front();

                // A hysteresis factor will be in effect until the total infection goes below the hysteresis factor;
                // until that happens the information required to return a movement factor must be kept in above variables.

                // Get the threshold of the next correction factor; otherwise the current correction factor can remain in
                // effect if the total infections never goes below the lower bound hysteresis factor, but also if it goes
                // above the original total infection threshold!
                auto next_pair_iterator = std::find(correction_factors.begin(), correction_factors.end(), pair);
                assert(next_pair_iterator != correction_factors.end());

                // If there is a next correction factor (for a higher total infection), then use it's total infection threshold
                if(std::distance(correction_factors.begin(), next_pair_iterator) != correction_factors.size() - 1) {
                    ++next_pair_iterator;
                }

                hysteresisFactor.in_effect = true;
                hysteresisFactor.infections_higher_bound = next_pair_iterator->first;
                hysteresisFactor.infections_lower_bound = pair.first - pair.second.back();
                hysteresisFactor.mobility_correction_factor = pair.second.front();
            }
            else {
                break;
            }
        }

        return correction;
    }
};

#endif //PANDEMIC_HOYA_2002_ZHONG_CELL_HPP
