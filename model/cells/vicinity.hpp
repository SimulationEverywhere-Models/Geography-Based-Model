//
// Created by binybrion on 6/29/20.
//

#ifndef CELL_DEVS_ZHONG_DEVEL_VICINITY_H
#define CELL_DEVS_ZHONG_DEVEL_VICINITY_H

#include <functional>
#include <cmath>
#include <nlohmann/json.hpp>

struct vicinity
{
    double correlation = 1.0f;

    explicit vicinity(double correlation) : correlation{correlation} {}

    vicinity(){}
};

void from_json(const nlohmann::json &json, vicinity &vicinity)
{
   json.at("correlation").get_to(vicinity.correlation);
}

#endif //CELL_DEVS_ZHONG_DEVEL_VICINITY_H