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
    Vicinity() = default;

    Vicinity(float borderLength, float borderLengthAdjacentRegion, unsigned int numberSharedRoads, float sharedBorderLength)
            :
            borderLength{borderLength},
            borderLengthAdjacentRegion{borderLengthAdjacentRegion},
            numberSharedRoads{numberSharedRoads},
            sharedBoundaryLength{sharedBorderLength}
    {
        computeWeightFactor(defaultWeightComputationFunction);

        computeCorrelationFactor(defaultCorrelationTransformationFunction);
    }

    void computeCorrelationFactor(const std::function<float(float)> &transformationFunction)
    {
        correlationFactor = transformationFunction(weightFactor);
    }

    /**
     * Calculate the weight factor given the geographical information between two regions.
     *
     * @param computationFunction a function that computes the weight factor based off of the following parameters, in order:
     *                              1. The length of the border of the region this vicinity is representing
     *                              2. The length of the border of the other region
     *                              3. The shared border length between the two aforementioned regions
     *                              4. The number of interconnection roads between the two aforementioned regions
     */
    void computeWeightFactor(const std::function<float(float, float, float, unsigned int)> &computationFunction)
    {
        weightFactor = computationFunction(borderLength, borderLengthAdjacentRegion, sharedBoundaryLength, numberSharedRoads);
    }

    static float defaultWeightComputationFunction(float borderLength_Region1, float borderLengthRegion2, float sharedBorderLength, unsigned int numberSharedRoads)
    {
        if(sharedBorderLength == 0)
        {
            if(numberSharedRoads == 0)
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
            float correlationFactor = (sharedBorderLength / borderLength_Region1 + sharedBorderLength / borderLengthRegion2) / 2.0f;

            if(numberSharedRoads == 0)
            {
                return correlationFactor;
            }
            else
            {
                return std::max(0.3f, correlationFactor);
            }
        }
    }

    static float defaultCorrelationTransformationFunction(float weightFactor)
    {
        return expf(-1.0f / weightFactor);
    }

    float correlationFactor;
    float borderLength;
    float borderLengthAdjacentRegion;
    unsigned int numberSharedRoads;
    float sharedBoundaryLength;

    float weightFactor;
};

void from_json(const nlohmann::json &json, Vicinity &vicinity)
{
    json.at("borderLength").get_to(vicinity.borderLength);
    json.at("borderLengthAdjacentRegion").get_to(vicinity.borderLengthAdjacentRegion);
    json.at("numberSharedRoads").get_to(vicinity.numberSharedRoads);
    json.at("sharedBorderLength").get_to(vicinity.sharedBoundaryLength);

    vicinity.computeWeightFactor(Vicinity::defaultWeightComputationFunction);
    vicinity.computeCorrelationFactor(Vicinity::defaultCorrelationTransformationFunction);

    static int i = 0;

    printf("%d \n", i++);
}

#endif //CELL_DEVS_ZHONG_DEVEL_VICINITY_H