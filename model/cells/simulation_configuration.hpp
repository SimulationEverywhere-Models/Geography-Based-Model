//
// Created by binybrion on 7/3/20.
//

#ifndef PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
#define PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP

#include <nlohmann/json.hpp>

struct simulation_configuration
{
    int phase_duration;
    double disobedient;
    int precision;
    using phase_rates =
                        std::vector<            // The age sub_division
                        std::vector<double>>;   // The stage of infection

    phase_rates virulence_rates;
    phase_rates recovery_rates;
    phase_rates mobility_rates;
    std::vector<float> mobility_correction_factor;
};

void from_json(const nlohmann::json& j, simulation_configuration &v) {

    j.at("disobedient").get_to(v.disobedient);
    j.at("precision").get_to(v.precision);
    j.at("virulence_rates").get_to(v.virulence_rates);
    j.at("recovery_rates").get_to(v.recovery_rates);
    j.at("mobility_rates").get_to(v.mobility_rates);
    j.at("mobility_correction_factor").get_to(v.mobility_correction_factor);
}

#endif //PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
