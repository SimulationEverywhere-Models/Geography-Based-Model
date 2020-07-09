//
// Created by binybrion on 6/30/20.
//

#ifndef PANDEMIC_HOYA_2002_SIR_HPP
#define PANDEMIC_HOYA_2002_SIR_HPP

#include <iostream>
#include <nlohmann/json.hpp>

struct sir {
    unsigned int population_density;
    unsigned int phase;
    std::vector<double> age_divided_populations;
    std::vector<double> infected;
    std::vector<double> recovered;

    double border_length;
    double land_area;
    double susceptible;

    sir(unsigned int popDensity, unsigned int phase, std::vector<double> age_divided_populations, std::vector<double> infected, std::vector<double> recovered, double border_length, double land_area)
            :
                population_density{popDensity},
                phase{phase},
                age_divided_populations{age_divided_populations},
                infected{std::move(infected)},
                recovered{std::move(recovered)},
                border_length{border_length},
                land_area{land_area} {
        susceptible = 1.0f - infected.front();

    }

    sir()
            :
                population_density{0},
                phase{0} {

    }

    unsigned int get_num_infected() const
    {
        return infected.size();
    }

    unsigned int get_num_recovered() const
    {
        return recovered.size();
    }

    bool operator!=(const sir &other) const {
        bool neq = population_density != other.population_density ||
                   phase != other.phase ||
                   susceptible != other.susceptible;

        return neq || (infected != other.infected) || (recovered != other.recovered) || (age_divided_populations != other.age_divided_populations);
    }
};

bool operator<(const sir &lhs, const sir &rhs) { return true; }

std::ostream &operator<<(std::ostream &os, const sir &sir) {
    os << "<" << sir.population_density << "," << sir.phase << "," << sir.get_num_infected() << "," << sir.get_num_recovered()
         << "," << sir.susceptible;

    for (auto infected : sir.infected) {
        os << "," << infected;
    }

    for (auto recovered : sir.recovered) {
        os << "," << recovered;
    }


    os << ">";

    return os;
}

void from_json(const nlohmann::json &json, sir &sir) {
    json.at("population_density").get_to(sir.population_density);
    json.at("infected").get_to(sir.infected);
    json.at("recovered").get_to(sir.recovered);
    json.at("age_divided_populations").get_to(sir.age_divided_populations);

    // Ensure that this variable is initialized as the default constructor is used here.
    sir.susceptible = 1.0f - sir.infected.front();
}

#endif //PANDEMIC_HOYA_2002_SIR_HPP
