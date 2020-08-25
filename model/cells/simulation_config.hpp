//
// Created by binybrion on 7/3/20.
//

#ifndef PANDEMIC_HOYA_2002_SIMULATION_CONFIG_HPP
#define PANDEMIC_HOYA_2002_SIMULATION_CONFIG_HPP

#include <nlohmann/json.hpp>
#include "distribution_type.h"
#include "phase_rates_object.h"

struct simulation_config
{
    int prec_divider;
    using phase_rates = std::vector<            // The age sub_division
                        std::vector<double>>;   // The stage of infection

    phase_rates virulence_rates;
    phase_rates recovery_rates;
    phase_rates mobility_rates;
    phase_rates fatality_rates;

    phase_rates_object random_virulence_rates;

    bool SIIRS_model = true;


};

std::pair<unsigned int, unsigned int> extract_indexes(const std::string &indexes) {

    std::stringstream indexes_stream{indexes};
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(indexes_stream, segment, '-')) {
        seglist.push_back(segment);
    }

    return {std::stoul(seglist[0], nullptr, 0), std::stoul(seglist[1], nullptr, 0)};
}

std::pair<std::string, double> get_distribution_information(const std::map<std::string, double> distribution_information) {

    return {distribution_information.begin()->first, distribution_information.begin()->second};
}

std::vector<distribution_relevant_indexes> create_distribution_indexes(std::map<std::string, std::map<std::string, double>> &distributions,
                                                                        const std::vector<std::vector<double>> &phase_rates) {

    std::vector<distribution_relevant_indexes> distribution_indexes;

    for(const auto &i : distributions) {

        auto indexes = get_distribution_information()
        auto distribution_information = get_distribution_information(i.second);

        if(distribution_information.first == "Normal") {
            std::normal_distribution<float> normal_distribution{};
        }
    }
}

void from_json(const nlohmann::json& json, simulation_config &v) {

    json.at("precision").get_to(v.prec_divider);
    json.at("virulence_rates").get_to(v.virulence_rates);
    json.at("recovery_rates").get_to(v.recovery_rates);
    json.at("mobility_rates").get_to(v.mobility_rates);
    json.at("fatality_rates").get_to(v.fatality_rates);
    json.at("SIIRS_model").get_to(v.SIIRS_model);

    std::string distribution_type_string;
    json.at("distribution_virulence").get_to(distribution_type_string);

    std::map<std::string, std::map<std::string, double>> virulence_distribution;

    json.at("distributions_virulence").get_to(virulence_distribution);

    for(const auto &i : virulence_distribution) {


    }


    for(int i = 0; i < v.recovery_rates.size(); ++i) {
        for(int k = 0; k < v.recovery_rates.at(i).size(); ++k){
            // A sum of greater than one refers to more than the entire population of an infection stage.
            assert(v.recovery_rates.at(i).at(k) + v.fatality_rates.at(i).at(k) <= 1.0f && "The recovery rate + fatality rate cannot exceed 1!");
        }
        assert(v.fatality_rates.at(i).back() <= 1.0f && "The fatality rate cannot exceed one!"); // Assert because the recovery rate has
                                                                                               // one less entry than the fatality rates.
    }
}

#endif //PANDEMIC_HOYA_2002_SIMULATION_CONFIG_HPP
