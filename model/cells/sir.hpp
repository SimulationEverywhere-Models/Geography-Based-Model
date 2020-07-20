//
// Created by binybrion on 6/30/20.
//

#ifndef PANDEMIC_HOYA_2002_SIR_HPP
#define PANDEMIC_HOYA_2002_SIR_HPP

#include <iostream>
#include <nlohmann/json.hpp>

struct sir {
    std::vector<double> susceptible;
    std::vector<std::vector<double>> infected;
    std::vector<std::vector<double>> recovered;

    // Required for the JSON library, as types used with it must be default-constructable.
    // The overloaded constructor results in a default constructor having to be manually written.
    sir() = default;

    sir(unsigned int popDensity, unsigned int phase, std::vector<double> susceptible,
            std::vector<double> infected, std::vector<double> recovered, double border_length, double land_area) :
                susceptible{std::move(susceptible)},
                infected{std::move(infected)},
                recovered{std::move(recovered)} {

    }

    unsigned int get_num_age_segments() const {
        return susceptible.size(); // Could use infections.size() or recovered.size(); simply a matter of preference.
    }

    unsigned int get_num_infected_phases() const {
        return infected.front().size(); // There will always be at least one age group, meaning the .front() call is always valid.
    }

    unsigned int get_num_recovered_phases() const {
        return recovered.front().size(); // There will always be at least one age group, meaning the .front() call is always valid.
    }

    bool operator!=(const sir &other) const {
        return (susceptible != other.susceptible) || (infected != other.infected) || (recovered != other.recovered);
    }
};

bool operator<(const sir &lhs, const sir &rhs) { return true; }

std::ostream &operator<<(std::ostream &os, const sir &sir) {

    std::string susceptible_information;
    std::string infected_information;
    std::string recovered_information;

    for(auto susceptible_age_segment : sir.susceptible) {
        susceptible_information += "," + std::to_string(susceptible_age_segment);
    }

    for(const auto &infected_age_segment : sir.infected) {
        for (auto infected_phase : infected_age_segment) {
            infected_information += "," + std::to_string(infected_phase);
        }
    }

    for(const auto &recovered_age_segment : sir.recovered) {
        for (auto recovered_phase : recovered_age_segment) {
            recovered_information += "," + std::to_string(recovered_phase);
        }
    }

    os << "<" << "," << sir.get_num_age_segments() << "," << sir.get_num_infected_phases() << "," << sir.get_num_recovered_phases();

    os << susceptible_information << infected_information << recovered_information << ">";

    return os;
}

void from_json(const nlohmann::json &json, sir &sir) {
    json.at("infected").get_to(sir.infected);
    json.at("recovered").get_to(sir.recovered);
    json.at("susceptible").get_to(sir.susceptible);
}

#endif //PANDEMIC_HOYA_2002_SIR_HPP
