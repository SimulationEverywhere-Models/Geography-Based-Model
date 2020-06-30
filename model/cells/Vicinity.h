//
// Created by binybrion on 6/29/20.
//

#ifndef CELL_DEVS_ZHONG_DEVEL_VICINITY_H
#define CELL_DEVS_ZHONG_DEVEL_VICINITY_H

#include <functional>
#include <cmath>

#include <nlohmann/json.hpp>

struct Vicinity
{

    Vicinity(std::string cell_id, float shared_border_length, unsigned int number_shared_roads)
            :
                cell_id{cell_id},
                number_shared_roads{number_shared_roads},
                shared_border_length{shared_border_length}
    {

    }

    Vicinity(){}

    void computeCorrelationFactor(float border_length, float adjacent_border_length)
    {
        correlationFactor = 1.0;

     //   correlationFactor = defaultCorrelationFactor(border_length, adjacent_border_length, shared_border_length, number_shared_roads);
    }

    static float defaultCorrelationFactor(float border_length, float adjacent_border_length, float shared_border_length, unsigned int number_shared_roads)
    {
        if(shared_border_length == 0)
        {
            if(number_shared_roads == 0)
            {
                return 0;
            }
            else
            {
                return 0.3f;
            }
        }
        else
        {
            float correlationFactor = (shared_border_length / border_length + shared_border_length / adjacent_border_length) / 2.0f;

            if(number_shared_roads == 0)
            {
                return correlationFactor;
            }
            else
            {
                return std::max(0.3f, correlationFactor);
            }
        }
    }

    std::string cell_id;
    float correlationFactor;
    unsigned int number_shared_roads;
    float shared_border_length;
};

void from_json(const nlohmann::json &json, Vicinity &vicinity)
{
    json.at("cell_id").get_to(vicinity.cell_id);
    json.at("shared_border_length").get_to(vicinity.shared_border_length);
    json.at("number_shared_roads").get_to(vicinity.number_shared_roads);
}

#endif //CELL_DEVS_ZHONG_DEVEL_VICINITY_H