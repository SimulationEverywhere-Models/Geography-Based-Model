//
// Created by binybrion on 6/29/20.
//

#ifndef PANDEMIC_HOYA_2002_ZHONG_CELL_HPP
#define PANDEMIC_HOYA_2002_ZHONG_CELL_HPP

#include <cmath>
#include <iostream>
#include <vector>
#include <cadmium/celldevs/cell/cell.hpp>
#include <iomanip>
#include "vicinity.hpp"
#include "sir.hpp"

using namespace std;
using namespace cadmium::celldevs;

/************************************/
/******COMPLEX STATE STRUCTURE*******/
/************************************/
// TODO are you using this sir structure? If not, remove it
struct sir {
    unsigned int population;
    unsigned int phase;
    int num_inf;
    int num_rec;
    float susceptible;
    float initial_infected;
    vector<float> infected;
    vector<float> recovered;
    float deaths;

    sir() : population(0), phase(0), susceptible(1) {}  // a default constructor is required
    sir(unsigned int pop, unsigned int phase, int num_inf, int num_rec, float initial_infected) : population(pop), phase(phase), num_inf(num_inf), num_rec(num_rec) {

        initialize();
    }

    void initialize()
    {
        infected.reserve(num_inf);
        recovered.reserve(num_rec);

        infected.push_back(initial_infected);
        susceptible = 1 - initial_infected;

        for(int i=1; i < num_inf; i++) {
            infected.push_back(0);
        }

        for(int i=0; i < num_rec; i++) {
            recovered.push_back(0);
        }

        deaths = 0;
    }
};

void from_json(const nlohmann::json &json, sir &sir)
{
    json.at("population").get_to(sir.population);
    json.at("susceptible").get_to(sir.susceptible);
    json.at("infected").get_to(sir.infected);
    json.at("recovered").get_to(sir.recovered);
    json.at("num_infected").get_to(sir.num_inf);
    json.at("num_recovered").get_to(sir.num_rec);
    json.at("initial_infected").get_to(sir.initial_infected);

    sir.initialize();
}

// Required for comparing states and detect any change
inline bool operator != (const sir &x, const sir &y) {
    bool neq = x.population != y.population || x.phase != y.phase || x.susceptible != y.susceptible || x.deaths != y.deaths;

    int i = 0;
    while(!neq and i < x.num_inf) {
        neq = x.infected[i] != y.infected[i];
        i += 1;
    }

    i = 0;
    while(!neq and i < x.num_rec) {
        neq = x.recovered[i] != y.recovered[i];
        i += 1;
    }

    return neq;
}
// Required if you want to use transport delay (priority queue has to sort messages somehow)
inline bool operator < (const sir& lhs, const sir& rhs){ return true; }
// Required for printing the state of the cell
std::ostream &operator << (std::ostream &os, const sir &x) {
    os << "<" << x.population << "," << x.phase << "," << x.num_inf << "," << x.num_rec << "," << x.susceptible;

    for(int i=0; i < x.num_inf; i++) {
        os << "," << x.infected[i];
    }

    for(int i=0; i < x.num_rec; i++) {
        os << "," << x.recovered[i];
    }

    os << "," << x.deaths;
    os << ">";
    return os;
}

/************************************/
/*****COMPLEX VICINITY STRUCTURE*****/
/************************************/
//struct Vicinity {
//    float connection;
//    Vicinity() : connection(0) {}  // a default constructor is required
//    Vicinity(float c) : connection(c) {}
//};
//
//
//void from_json(const nlohmann::json &json, Vicinity &Vicinity)
//{
//    json.at("connection").get_to(Vicinity.connection);
//}

struct vr {
    float virulence;
    float recovery;
    vr(): virulence(0.6), recovery(0.01) {}
    vr(float v, float r): virulence(v), recovery(r) {}
};

void from_json(const nlohmann::json& j, vr &v) {
    j.at("virulence").get_to(v.virulence);
    j.at("recovery").get_to(v.recovery);
}


template <typename T>
    class zhong_cell : public cell<T, std::string, SIR, Vicinity> {
    public:

            template<typename X>
            using cell_unordered = std::unordered_map<std::string, X>;

            using cell<T, std::string, SIR, Vicinity>::simulation_clock;
            using cell<T, std::string, SIR, Vicinity>::state;
            using cell<T, std::string, SIR, Vicinity>::neighbors;

        using config_type = vr;
        float virulence;
        float recovery;

        // This is the number of phases of a lock down.
        static const int NUM_PHASES = 1;

        // This is the sum of all of the infected days (incubation, actual infection, and latency).
        static const int INF_NUM = 16;

        float disobedient = 0.0;
        int precDivider = 1000;

        // For every day of the lock down and for every day that a person is infected, assign various values.
        float virulencyRates[NUM_PHASES][INF_NUM] = {{0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15, 0.15}};
        float recoveryRates[NUM_PHASES][INF_NUM-1] = {{0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07, 0.07}};
        float mortalityRates[NUM_PHASES][INF_NUM] = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
        float movilityRates[NUM_PHASES][INF_NUM] = {{0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6}};

        zhong_cell() : cell<T, std::string, SIR, Vicinity>() {}

        zhong_cell(std::string const &cell_id, cell_unordered<Vicinity> const &neighborhood, SIR initial_state, std::string const &delay_id, vr config) :
        cell<T, std::string, SIR, Vicinity>(cell_id, neighborhood, initial_state, delay_id) {
            virulence = config.virulence;
            recovery = config.recovery;
        }

        // user must define this function. It returns the next cell state and its corresponding timeout
        SIR local_computation() const override {

            SIR res = state.current_state;

            for(auto neighbor : neighbors)
            {
                SIR neighbourCountryState = state.neighbors_state.at(neighbor);
                Vicinity v = state.neighbors_vicinity.at(neighbor);

                v.computeCorrelationFactor(res.border_length, neighbourCountryState.border_length);
            }


            float new_i = std::round(get_phase_penalty(res.phase) * new_infections() * precDivider) / precDivider;

            // Of the population that is on the last day of the infection, they are now considered recovered.
            float new_r = res.infected[res.num_inf-1];

            float new_s = 1;

            // Equation 6e
            for (int i = 0; i < res.num_inf - 1; ++i)
            {
                // Calculate all of the new recovered- for every day that a population is infected, some recover.
                new_r += std::round(res.infected[i] * recoveryRates[res.phase][i] * precDivider) / precDivider;
            }

            // Equation 6b is done through several steps, every time new_s is subtracted from

            // Equation 6d
            for (int i = res.num_inf - 1; i > 0; --i)
            {
                // *** Calculate proportion of infected on a given day of the infection ***

                // The previous day of infection
                float curr_inf = res.infected[i-1];

                // A proportion of the previous day's infection recovers, leading to a new proportion
                // of the population that is currently infected at the current day of infection
                curr_inf *= 1 - recoveryRates[res.phase][i-1];

                res.infected[i] = std::round(curr_inf * precDivider) / precDivider;

                // The amount of susceptible does not include the amount the infected proportion of the population
                new_s -= res.infected[i];
            }

            // The people on the first day of infection are equal to the number of infections from the susceptible population
            res.infected[0] = new_i;

            // The susceptible population does not include
            new_s -= new_i;

            // Equation 6a
            for (int i = res.num_rec - 1; i > 0; --i)
            {
                // Each day of the recovered is the value of the previous day. The population on the last day is
                // now susceptible; this is implicitly done already as the susceptible value was set to 1.0 and the
                // population on the last day of recovery is never subtracted from the susceptible value.
                res.recovered[i] = res.recovered[i-1];
                new_s -= res.recovered[i];
            }

            // The people on the first day of recovery are those that were on the last stage of infection
            // in the previous time step.
            res.recovered[0] = new_r;

            // The susceptible population does not include the recovered population
            new_s -= new_r;

            if(new_s > -0.001 && new_s < 0) new_s = 0;  // float precision issues

            assert(new_s >= 0);
            res.susceptible = new_s;

            res.phase = next_phase(res.phase);

            return res;
        }
        // It returns the delay to communicate cell's new state.
        T output_delay(SIR const &cell_state) const override {
            return 1;
        }

        void check_valid_vicinity() {
            auto vicinities = state.neighbors_vicinity;
            for (auto neighbor: vicinities) {
                Vicinity v = neighbor.second;
                assert(v.correlationFactor >= 0 && v.correlationFactor <= 1);
                //assert(v.movement >= 0 && v.movement <= 1);
            }
        }

        void check_valid_state() const {
            SIR init = state.current_state;
            float sum = init.susceptible;
            assert(init.susceptible >= 0 && init.susceptible <= 1);
            for(int i=0; i < init.num_inf; i++) {
                assert(init.infected[i] >= 0 && init.infected[i] <= 1);
                sum += init.infected[i];
            }
            for(int i=0; i < init.num_rec; i++) {
                assert(init.recovered[i] >= 0 && init.recovered[i] <= 1);
                sum += init.recovered[i];
            }
            assert(sum == 1);
        }

        float new_infections() const {
            float inf = 0;
            SIR cstate = state.current_state;

            // internal infected
            for (int i = 0; i < cstate.num_inf; ++i) {
                inf += virulencyRates[cstate.phase][i] * cstate.infected[i];
            }
            inf *= cstate.susceptible;

            // external infected
            for(auto neighbor: neighbors) {
                SIR nstate = state.neighbors_state.at(neighbor);
                Vicinity v = state.neighbors_vicinity.at(neighbor);

                for (int i = 0; i < nstate.num_inf; ++i) {
                    inf += v.correlationFactor * movilityRates[cstate.phase][i] * cstate.susceptible *
                           ((float)nstate.population / (float)cstate.population) *
                           virulencyRates[cstate.phase][i] * nstate.infected[i];
                }
            }

            return std::min(cstate.susceptible, inf);
        }

        float get_phase_penalty(unsigned int phase) const {
            assert(0 <= phase && phase < NUM_PHASES);
            return 1;
        }

        unsigned int next_phase(unsigned int phase) const {
            return 0;
        }
};

#endif //PANDEMIC_HOYA_2002_ZHONG_CELL_HPP
