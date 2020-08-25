//
// Created by binybrion on 8/25/20.
//

#ifndef PANDEMIC_GEOGRAPHICAL_MODEL_PHASE_RATES_OBJECT_H
#define PANDEMIC_GEOGRAPHICAL_MODEL_PHASE_RATES_OBJECT_H

#include <memory>
#include <vector>
#include "distribution_type.h"

class distribution_relevant_indexes
{
    public:

        distribution_relevant_indexes(distribution_base distribution, unsigned int lower_index, unsigned int higher_index)
                                :
                distribution{distribution},
                                    lower_index{lower_index},
                                    higher_index{higher_index} {

        }

        bool within_relevant_index(unsigned int index) const {
            return lower_index <= index && index < higher_index;
        }

        double get_value() const {
            return distribution.get_value();
        }

    private:

        mutable distribution_base distribution;
        unsigned int lower_index;
        unsigned int higher_index;
};

class phase_rates_object
{
    public:


        void set_distribution_indexes(std::vector<std::vector<distribution_relevant_indexes>> &distribution_indexes) {
            this->distribution_indexes = std::move(distribution_indexes);
        }

        double get_value_at(unsigned int age_group_index, unsigned int phase_index) const {
            for(const auto &i : distribution_indexes.at(age_group_index)) {
                if(i.within_relevant_index(phase_index)) {
                    return i.get_value();
                }
            }

            throw std::runtime_error{"No valid phase index of: " + std::to_string(phase_index)};
        }

    private:

        std::vector<std::vector<distribution_relevant_indexes>> distribution_indexes;
};


#endif //PANDEMIC_GEOGRAPHICAL_MODEL_PHASE_RATES_OBJECT_H
