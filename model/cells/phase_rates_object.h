//
// Created by binybrion on 8/25/20.
//

#ifndef PANDEMIC_GEOGRAPHICAL_MODEL_PHASE_RATES_OBJECT_H
#define PANDEMIC_GEOGRAPHICAL_MODEL_PHASE_RATES_OBJECT_H

#include <memory>
#include <vector>
#include "distribution_type.h"

/// Encapsulates all of the phase rates for different age groups
class phase_rates_object
{
    private:
        // Mutable as the get_value_at() function must be const, but certain distributions,
        // such as normal, internally mutate their state when getting a value from them.
        mutable std::vector<std::vector<distribution>> distributions;
    public:

        void set_distribution_indexes(std::vector<std::vector<distribution>> &distributions) {
            this->distributions = std::move(distributions);
        }

        double get_value_at(unsigned int age_group_index, unsigned int phase_index) const {
            for(auto &i : distributions.at(age_group_index)) {
                if(i.within_relevant_index(phase_index)) {
                    return i.get_value();
                }
            }
            throw std::runtime_error{"No valid phase index of: " + std::to_string(phase_index)};
        }
};

#endif //PANDEMIC_GEOGRAPHICAL_MODEL_PHASE_RATES_OBJECT_H
