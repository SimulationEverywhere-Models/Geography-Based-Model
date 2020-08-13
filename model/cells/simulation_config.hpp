//
// Created by binybrion on 7/3/20.
//

#ifndef PANDEMIC_HOYA_2002_SIMULATION_CONFIG_HPP
#define PANDEMIC_HOYA_2002_SIMULATION_CONFIG_HPP

#include <nlohmann/json.hpp>

struct simulation_config
{
    // To make the parameters of the correction_factors variable more obvious
    using infection_threshold = float;
    using mobility_correction_factor = std::array<float, 2>; // The first value is the mobility correction factor;
                                                             // The second one is the hysteresis factor.

    std::map<infection_threshold, mobility_correction_factor> correction_factors;
    double disobedient;
    int precision;
    using phase_rates = std::vector<            // The age sub_division
                        std::vector<double>>;   // The stage of infection

    phase_rates virulence_rates;
    phase_rates recovery_rates;
    phase_rates mobility_rates;
    phase_rates fatality_rates;

    double hospital_infected_capacity;
    double over_capacity_fatality_modifier;
};

void from_json(const nlohmann::json& j, simulation_config &v) {
    // TODO just for you to know: you can define default fields, so if it is not defined in the JSON, you take a default
    // This is just an example, remove it if you don't like this idea
    if (j.contains("disobedient"))
        j.at("disobedient").get_to(v.disobedient);
    else
        v.disobedient = 0;
    j.at("precision").get_to(v.precision);
    j.at("virulence_rates").get_to(v.virulence_rates);
    j.at("recovery_rates").get_to(v.recovery_rates);
    j.at("mobility_rates").get_to(v.mobility_rates);
    j.at("fatality_rates").get_to(v.fatality_rates);
    j.at("hospital_infected_capacity").get_to(v.hospital_infected_capacity);
    j.at("over_capacity_fatality_modifier").get_to(v.over_capacity_fatality_modifier);

    for(int i = 0; i < v.recovery_rates.size(); ++i) {
        for(int k = 0; k < v.recovery_rates[i].size(); ++k){
            // A sum of greater than one refers to more than the entire population of an infection stage.
            assert(v.recovery_rates[i][k] + v.fatality_rates[i][k] <= 1.0f && "The recovery rate + fatality rate cannot exceed 1!");
        }
        assert(v.fatality_rates[i].back() <= 1.0f && "The fatality rate cannot exceed one!"); // Assert because the recovery rate has
                                                                                               // one less entry than the fatality rates.
    }
}

#endif //PANDEMIC_HOYA_2002_SIMULATION_CONFIG_HPP
