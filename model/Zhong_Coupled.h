//
// Created by binybrion on 6/29/20.
//

#ifndef PANDEMIC_HOYA_2002_ZHONG_COUPLED_H
#define PANDEMIC_HOYA_2002_ZHONG_COUPLED_H

#include <nlohmann/json.hpp>
#include <cadmium/celldevs/coupled/grid_coupled.hpp>
#include "cells/Zhong_Cell.h"

template <typename T>
class Zhong_Coupled : public cadmium::celldevs::grid_coupled<T, sir, Vicinity>
{
    public:

        explicit Zhong_Coupled(std::string const &id) : grid_coupled<T, sir, Vicinity>(id)
        {}

        template<typename X>
        using cell_unordered = std::unordered_map<std::string, X>;

        void add_grid_cell_json(std::string const &cell_type, cell_map<sir, Vicinity> &map, std::string const &delay_id,
                                nlohmann::json const &config) override
        {
            if (cell_type == "hoya")
            {
                auto conf = config.get<typename zhong_cell<T>::config_type>();
                this->template add_cell<zhong_cell>(map, delay_id, conf);
            } else throw std::bad_typeid();
        }
};

#endif //PANDEMIC_HOYA_2002_ZHONG_COUPLED_H
