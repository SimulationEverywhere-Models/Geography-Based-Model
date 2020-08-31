//
// Created by binybrion on 8/25/20.
//

#ifndef PANDEMIC_GEOGRAPHICAL_MODEL_DISTRIBUTION_TYPE_H
#define PANDEMIC_GEOGRAPHICAL_MODEL_DISTRIBUTION_TYPE_H

#include <random>

enum class distribution_type {
    Fixed,
    Normal,
};

/// Keeps track of what distribution to use when a value is required
struct distribution {
private:

    std::pair<unsigned int, unsigned int> valid_phase_rate_indexes;
    std::pair<double, double> max_range_values;
    distribution_type distribution_id;
    std::default_random_engine random_engine;

    std::normal_distribution<double> normal_distribution;
public:
    explicit distribution(std::pair<unsigned int, unsigned int> valid_phase_indexes, std::pair<double, double> max_range_values)
                            :
                                valid_phase_rate_indexes{valid_phase_indexes},
                                max_range_values{max_range_values},
                                distribution_id{distribution_type::Fixed} {

    }

     distribution(std::pair<unsigned int, unsigned int> valid_phase_indexes, std::pair<double, double> max_range_values, unsigned int seed_value, std::normal_distribution<double> distribution)
                                :
                                    valid_phase_rate_indexes{valid_phase_indexes},
                                    max_range_values{max_range_values},
                                    random_engine{seed_value},
                                    normal_distribution{distribution},
                                    distribution_id{distribution_type::Normal} {

    }

    bool within_relevant_index(unsigned int index) const {
        return valid_phase_rate_indexes.first <= index && index < valid_phase_rate_indexes.second;
    }

    double get_value() {
         double value = 0.0;
         switch(distribution_id) {
             case distribution_type ::Normal:
                value = normal_distribution(random_engine);
                break;
             default:  // Fixed
                 break;
         }

         // Ensure that the given value is within the acceptable value; if it is not, then return the value that is closest
         // to an acceptable value.
         // TODO aren't the max values 0 and 1 always?
         // TODO Why don't you do: return std::min(max_range_values.second, std::max(max_range_values.first, value));
         if(value < max_range_values.first || value > max_range_values.second) {
            if(std::abs(value - max_range_values.first) < std::abs(value - max_range_values.second)) {
                return max_range_values.first;
            }
            else {
                return max_range_values.second;
            }
        }
        return value;
    }
};

#endif //PANDEMIC_GEOGRAPHICAL_MODEL_DISTRIBUTION_TYPE_H
