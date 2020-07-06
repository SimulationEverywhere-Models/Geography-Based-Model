//
// Created by binybrion on 7/3/20.
//

#ifndef PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
#define PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP

#include <nlohmann/json.hpp>

struct simulation_configuration
{
    float disobedient;
    int precision;
    std::vector<std::vector<float>> virulence_rates;
    std::vector<std::vector<float>> recovery_rates;
    std::vector<std::vector<float>> mobility_rates;
};

void from_json(const nlohmann::json& j, simulation_configuration &v) {

    j.at("disobedient").get_to(v.disobedient);
    j.at("precision").get_to(v.precision);
    j.at("virulence_rates").get_to(v.virulence_rates);
    j.at("recovery_rates").get_to(v.recovery_rates);
    j.at("mobility_rates").get_to(v.mobility_rates);
}

#endif //PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
