//
// Created by binybrion on 6/29/20.
//

#ifndef PANDEMIC_HOYA_2002_ZHONG_COUPLED_H
#define PANDEMIC_HOYA_2002_ZHONG_COUPLED_H

#include <nlohmann/json.hpp>
#include <cadmium/celldevs/coupled/cells_coupled.hpp>
#include "cells/Zhong_Cell.h"

template <typename T>
class Zhong_Coupled : public cadmium::celldevs::cells_coupled<T, std::string, SIR, Vicinity>
{
    public:

        explicit Zhong_Coupled(std::string const &id) : cells_coupled<T, std::string, SIR, Vicinity>(id)
        {}

        template<typename X>
        using cell_unordered = std::unordered_map<std::string, X>;

        void add_cell_json(std::string const &cell_type, std::string const &cell_id,
                           cell_unordered<Vicinity> const &neighborhood,
                           SIR initial_state,
                           std::string const &delay_id,
                           nlohmann::json const &config) override
        {
            if (cell_type == "zhong")
            {
                auto conf = config.get<typename zhong_cell<T>::config_type>();
                this->template add_cell<zhong_cell>(cell_id, neighborhood, initial_state, delay_id, conf);
            } else throw std::bad_typeid();
        }
};

#endif //PANDEMIC_HOYA_2002_ZHONG_COUPLED_H
