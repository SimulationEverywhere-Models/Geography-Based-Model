//
// Created by binybrion on 6/30/20.
//

#ifndef PANDEMIC_HOYA_2002_SIR_HPP
#define PANDEMIC_HOYA_2002_SIR_HPP

#include <iostream>
#include <nlohmann/json.hpp>

struct sir {
    std::vector<double> age_group_proportions;
    std::vector<double> susceptible;
    std::vector<std::vector<double>> infected;
    std::vector<std::vector<double>> recovered;
    std::vector<double> fatalities;

    // Required for the JSON library, as types used with it must be default-constructable.
    // The overloaded constructor results in a default constructor having to be manually written.
    sir() = default;

    sir(unsigned int popDensity, unsigned int phase, std::vector<double> susceptible,
            std::vector<double> infected, std::vector<double> recovered, double fatalities) :
                susceptible{std::move(susceptible)},
                infected{std::move(infected)},
                recovered{std::move(recovered)},
                fatalities{fatalities} {

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

    static double sum_state_vector(const std::vector<double> &state_vector) {
        return std::accumulate(state_vector.begin(), state_vector.end(), 0.0f);
    }

    // For the get_total_XXX functions, remember that the sum of the values in each vector associated with an age group
    // is one. When looking at the population as a whole, the sum of any state vector has to be adjusted according to how
    // big of a proportion the age group contributes to a population.

    double get_total_fatalities() const {
        double total_fatalities = 0.0f;
        for(int i = 0; i < age_group_proportions.size(); ++i) {
            total_fatalities += fatalities[i] * age_group_proportions[i];
        }
        return total_fatalities;
    }

    double get_total_infections() const {
        float total_infections = 0.0f;
        for(int i = 0; i < age_group_proportions.size(); ++i) {
            total_infections += sum_state_vector(infected[i]) * age_group_proportions[i];
        }
        return total_infections;
    }

    double get_total_recovered() const {
        double total_recoveries = 0.0f;
        for(int i = 0; i < age_group_proportions.size(); ++i) {
            total_recoveries += sum_state_vector(recovered[i]) * age_group_proportions[i];
        }
        return total_recoveries;
    }

    double get_total_susceptible() const {
        double total_susceptible = 0.0f;
        for(int i = 0; i < age_group_proportions.size(); ++i) {
            total_susceptible += susceptible[i] * age_group_proportions[i];
        }
        return total_susceptible;
    }

    bool operator!=(const sir &other) const {
        return (susceptible != other.susceptible) || (infected != other.infected) || (recovered != other.recovered);
    }
};

bool operator<(const sir &lhs, const sir &rhs) { return true; }

std::ostream &operator<<(std::ostream &os, const sir &sir) {

    bool print_specific_state_information = true;

    if(print_specific_state_information) {
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

        // First two numbers are irrelevant when running the notebook at this time.
        os << "<" << sir.get_num_age_segments() << ",0" << "," << sir.get_num_infected_phases() << "," << sir.get_num_recovered_phases() << ",";

        os << sir.get_total_susceptible() << infected_information << recovered_information << ">";
    }
    else {


        float new_infections = 0.0f;
        float new_recoveries = 0.0f;

        for(int i = 0; i < sir.age_group_proportions.size(); ++i) {
            new_infections += sir.infected.at(i).at(0) * sir.age_group_proportions.at(i);
            new_recoveries += sir.recovered.at(i).at(0) * sir.age_group_proportions.at(i);
        }

        os << "<" << sir.get_total_susceptible() << "," << sir.get_total_infections() << "," << sir.get_total_recovered() << "," << new_infections << "," << new_recoveries << "," << sir.get_total_fatalities() << ">";
    }

    return os;
}

void from_json(const nlohmann::json &json, sir &sir) {
    json.at("age_group_proportions").get_to(sir.age_group_proportions);
    json.at("infected").get_to(sir.infected);
    json.at("recovered").get_to(sir.recovered);
    json.at("susceptible").get_to(sir.susceptible);
    json.at("fatalities").get_to(sir.fatalities);
    
    assert(sir.age_group_proportions.size() == sir.susceptible.size() && sir.age_group_proportions.size() == sir.infected.size()
           && sir.age_group_proportions.size() == sir.recovered.size() && "There must be an equal number of age groups between"
                                                                          "age_group_proportions, susceptible, infected, and recovered!\n");
}

#endif //PANDEMIC_HOYA_2002_SIR_HPP
