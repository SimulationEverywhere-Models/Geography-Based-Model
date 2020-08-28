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

/// Extracts the range of phases specified in the key of a distribution.
/// For example, '1-24' returns a pair of [1, 24).
std::pair<unsigned int, unsigned int> extract_indexes(const std::string &indexes) {

    std::stringstream indexes_stream{indexes};
    std::string segment;
    std::vector<std::string> seglist;

    const char range_delimiter = '-';

    // If the user specified a single phase rather than a range of phases, then the logic to create a range from that
    // single value is different. (For example, '1' instead of '1-23')
    if(indexes.find(range_delimiter) == std::string::npos) {
        bool valid_number = !indexes.empty() && indexes.find_first_not_of("0123456789") == std::string::npos;
        assert(valid_number);
        return {std::stoul(indexes, nullptr, 0), std::stoul(indexes, nullptr, 0)};
    }

    while(std::getline(indexes_stream, segment, '-')) {

        bool valid_number = !segment.empty() && segment.find_first_not_of("0123456789") == std::string::npos;
        assert(valid_number);
        seglist.push_back(segment);
    }

    return {std::stoul(seglist[0], nullptr, 0), std::stoul(seglist[1], nullptr, 0)};
}

/// Helper function to get rid of the fact that the required information is stored in a map.
std::tuple<std::string, unsigned int, double> get_distribution_information(const std::map<std::string, std::array<double, 2>> distribution_information) {

    // The map only has one entry, for example "normal": ...
    //                                          ^^^^
    //                                          single key
    return
            {
                distribution_information.begin()->first,
                distribution_information.begin()->second.front(),
                distribution_information.begin()->second.back()
            };
}

std::vector<distribution> create_distribution_indexes(std::map<std::string, std::map<std::string, std::array<double, 2>>> &distributions,
                                                                        const std::vector<double> &phase_rates,
                                                                        std::pair<double, double> max_range_values) {

    std::vector<distribution> distribution_indexes;

    // Keeps track of what phase rate indexes were specified by the user; any that were not specified are given the Fixed
    // distribution.
    std::vector<unsigned char> covered_indexes(phase_rates.size(), false);

    for(const auto &i : distributions) {

        auto indexes = extract_indexes(i.first);
        auto distribution_information = get_distribution_information(i.second);

        if(std::get<0>(distribution_information) == "normal") {

            std::normal_distribution<double> normal_distribution(phase_rates.at(indexes.first), std::get<2>(distribution_information));
            std::pair<unsigned int, unsigned int> valid_phase_rate_index;

            if(indexes.first == indexes.second) {
                valid_phase_rate_index = std::make_pair(indexes.first, indexes.first + 1);
            }
            else {
                valid_phase_rate_index = std::make_pair(indexes.first, indexes.second);
            }

            distribution_indexes.emplace_back(valid_phase_rate_index, max_range_values, std::get<1>(distribution_information), normal_distribution);
        }

        // Special case if user wrote a specific phase state index, instead of a range...
        if(indexes.first == indexes.second) {
            assert(!covered_indexes.at(indexes.first));
            covered_indexes.at(indexes.first) = true;
        }

        // ... because here the less than operator is used, so a range with one number would not enter the loop.
        // This is done to keep the general notation of a slice indices being [beginning, end)
        for(int j = indexes.first; j < indexes.second; ++j) {
            assert(!covered_indexes.at(j));
            covered_indexes.at(j) = true;
        }
    }

    // Any phase rates that were not covered, give them a Fixed distribution with the value of the phase rate index
    for(int i = 0; i < covered_indexes.size(); ++i) {
        if(!covered_indexes.at(i)) {
            distribution_indexes.emplace_back(std::make_pair(i, i + 1), std::make_pair(phase_rates.at(i), phase_rates.at(i))); // Creates a Fixed distribution
        }
    }

    return distribution_indexes;
}

void from_json(const nlohmann::json& json, simulation_config &v) {

    json.at("precision").get_to(v.prec_divider);
    json.at("virulence_rates").get_to(v.virulence_rates);
    json.at("recovery_rates").get_to(v.recovery_rates);
    json.at("mobility_rates").get_to(v.mobility_rates);
    json.at("fatality_rates").get_to(v.fatality_rates);
    json.at("SIIRS_model").get_to(v.SIIRS_model);

    std::vector<std::map<std::string, std::map<std::string, std::array<double, 2>>>> virulence_distribution;

    json.at("distributions_virulence").get_to(virulence_distribution);

    assert(virulence_distribution.size() == v.virulence_rates.size());

    // Keeps track of all the distributions to apply for ranges of stages of virulence rates for each age group
    std::vector<std::vector<distribution>> virulence_relevant_indexes;

    // Handle each age group one-by-one for clarity
    for(int i = 0; i < virulence_distribution.size(); ++i) {
        virulence_relevant_indexes.emplace_back(create_distribution_indexes(virulence_distribution[i], v.virulence_rates[i], {0, 1}));
    }

    v.random_virulence_rates.set_distribution_indexes(virulence_relevant_indexes);

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
