//
// Created by binybrion on 7/3/20.
//

#ifndef PANDEMIC_HOYA_2002_RATES_CONFIGURATION_H
#define PANDEMIC_HOYA_2002_RATES_CONFIGURATION_H

#include <nlohmann/json.hpp>

struct rates_configurations
{
    std::vector<float> virulency_rates;
    std::vector<float> recovery_rates;
    std::vector<float> mobility_rates;
};

void from_json(const nlohmann::json& j, rates_configurations &v) {

    j.at("virulency_rates").get_to(v.virulency_rates);
    j.at("recovery_rates").get_to(v.recovery_rates);
    j.at("mobility_rates").get_to(v.mobility_rates);
}

#endif //PANDEMIC_HOYA_2002_RATES_CONFIGURATION_H
