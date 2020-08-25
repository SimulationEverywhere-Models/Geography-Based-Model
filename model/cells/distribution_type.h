//
// Created by binybrion on 8/25/20.
//

#ifndef PANDEMIC_GEOGRAPHICAL_MODEL_DISTRIBUTION_TYPE_H
#define PANDEMIC_GEOGRAPHICAL_MODEL_DISTRIBUTION_TYPE_H

#include <random>

enum class distribution_type {
        Normal
};

struct distribution_base {

    explicit distribution_base(std::normal_distribution<double> distribution)
                                :
                                    normal_distribution{distribution},
                                    distribution_id{distribution_type::Normal} {

    }

    double get_value() {

        switch(distribution_id) {
            case distribution_type ::Normal:
                return normal_distribution(random_engine);
        }
    }

    private:

        distribution_type distribution_id;

        std::default_random_engine random_engine;
        std::normal_distribution<double> normal_distribution;
};

#endif //PANDEMIC_GEOGRAPHICAL_MODEL_DISTRIBUTION_TYPE_H
