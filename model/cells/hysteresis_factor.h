//
// Created by binybrion on 8/12/20.
//

#ifndef PANDEMIC_HOYA_2002_HYSTERESIS_FACTOR_H
#define PANDEMIC_HOYA_2002_HYSTERESIS_FACTOR_H

/**
 * Stores the state of the movement restriction that a cell imposes on a neighbouring cell due to that neighbouring
 * having a certain amount of their population being infected.
 */
struct hysteresis_factor
{
    bool hysteresis_in_effect = false;
    float hysteresis_mobility_correction_factor = 1.0f;
    float hysteresis_infections_higher_bound = 0.0f; // Infection threshold of next correction factor
    float hysteresis_infections_lower_bound = 0.0f; // Infection threshold of hysteresis adjusted current correction factor
};

#endif //PANDEMIC_HOYA_2002_HYSTERESIS_FACTOR_H
