//
// Created by binybrion on 6/30/20.
//

#ifndef PANDEMIC_HOYA_2002_SIR_HPP
#define PANDEMIC_HOYA_2002_SIR_HPP

#include <iostream>
#include <nlohmann/json.hpp>

struct sir {
    unsigned int population;
    unsigned int phase;
    int num_inf;
    int num_rec;
    float susceptible;
    float initial_infected;
    std::vector<float> infected;
    std::vector<float> recovered;
    float deaths;

    float border_length;
    float land_area;

    sir(unsigned int pop, unsigned int phase, std::vector<float> infected, std::vector<float> recovered, float border_length, float land_area)
            :
                population{pop},
                phase{phase},
                infected{std::move(infected)},
                recovered{std::move(recovered)},
                deaths{0},
                border_length{border_length},
                land_area{land_area} {
        initialize();
    }

    sir()
            :
                population{0},
                phase{0},
                susceptible{1} {

    }

    void initialize() {

        initial_infected = infected.front();

        num_inf = infected.size();

        num_rec = recovered.size();

        susceptible = 1 - initial_infected;
    }

    bool operator!=(const sir &other) {
        bool neq = population != other.population ||
                   phase != other.phase ||
                   susceptible != other.susceptible ||
                   deaths != other.deaths;

        int i = 0;

        while (!neq && i < num_inf) {
            neq = infected[i] != other.infected[i];

            i += 1;
        }

        i = 0;

        while (!neq && i < num_rec) {
            neq = recovered[i] != other.recovered[i];

            i += 1;
        }

        return neq;
    }
};

bool operator<(const sir &lhs, const sir &rhs) { return true; }

std::ostream &operator<<(std::ostream &os, const sir &sir) {
    os << "<" << sir.population << "," << sir.phase << "," << sir.num_inf << "," << sir.num_rec << ","
       << sir.susceptible;

    for (auto infected : sir.infected) {
        os << "," << infected;
    }

    for (auto recovered : sir.recovered) {
        os << "," << recovered;
    }

    os << "," << sir.deaths;

    os << ">";

    return os;
}

void from_json(const nlohmann::json &json, sir &sir) {
    json.at("population").get_to(sir.population);
    json.at("susceptible").get_to(sir.susceptible);
    json.at("infected").get_to(sir.infected);
    json.at("recovered").get_to(sir.recovered);

    sir.initialize();
}

#endif //PANDEMIC_HOYA_2002_SIR_HPP
