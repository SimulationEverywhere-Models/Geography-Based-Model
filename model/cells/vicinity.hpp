//
// Created by binybrion on 6/29/20.
//

#ifndef CELL_DEVS_ZHONG_DEVEL_VICINITY_H
#define CELL_DEVS_ZHONG_DEVEL_VICINITY_H

#include <functional>
#include <cmath>
#include <nlohmann/json.hpp>

// TODO Just style: call the header files with the .hpp extension, using snake_case naming

struct Vicinity
{
    // TODO just style: put the attributes of structs and classes before anything
    // TODO Doing so, before reaching the constructors and methods, I know what is the struct composed of
    // TODO Check the rest of the code to change it to this style guide
    float correlationFactor;
    unsigned int number_shared_roads;
    float shared_border_length;

    Vicinity(float shared_border_length, unsigned int number_shared_roads) :
    number_shared_roads{number_shared_roads},
    shared_border_length{shared_border_length}
    { }

    Vicinity(){}

    void computeCorrelationFactor(float border_length, float adjacent_border_length)
    // TODO Why is this function void? shouldn't it return the correlation?
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
};

void from_json(const nlohmann::json &json, Vicinity &vicinity)
{
    json.at("shared_border_length").get_to(vicinity.shared_border_length);
    json.at("number_shared_roads").get_to(vicinity.number_shared_roads);
}

#endif //CELL_DEVS_ZHONG_DEVEL_VICINITY_H