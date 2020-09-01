/**
 * Copyright (c) 2020, Román Cárdenas Rodríguez
 * ARSLab - Carleton University
 * GreenLSI - Polytechnic University of Madrid
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <random>
#include <algorithm>


// This is the abstract class -> all the coefficients inherit from this class
class coefficient {
public:
    coefficient() {}
    virtual ~coefficient() = default;
    virtual double get_coefficient() { return 0; }
};

// This class is for fixed coefficients -> the only attribute is the value of the coefficient
class fixed_coefficient : public coefficient {
    double value;
public:
    fixed_coefficient(double value) : coefficient(), value(value) {
        assert(value >= 0 && value <= 1);
    }

    // For using a factory pattern
    template<typename... Args>
    fixed_coefficient(Args&&... args) { assert(false && "Invalid constructor"); }

    double get_coefficient() override { return value; }
};

// This class is for gaussian/normal coefficients -> we need mean, variance, and seed?
class normal_coefficient : public coefficient {
    std::normal_distribution<double> distribution;
    std::default_random_engine random_engine;
public:
    // With seed...
    normal_coefficient(double mean_value, double stddev, double seed) : coefficient(), distribution(mean_value, stddev), random_engine(seed) {}

    // .. and without seed
    normal_coefficient(double mean_value, double stddev) : coefficient(), distribution(mean_value, stddev), random_engine() {}

    // For using a factory pattern
    template<typename... Args>
    normal_coefficient(Args&&... args) { assert(false && "Invalid constructor"); }

    double get_coefficient() override {
        double value = distribution(random_engine);
        return std::min(1.0, std::max(0.0, value));
    }
};

// And this is the coefficient generator factory -> in the main file, while parsing the JSON, we generate pointers to these generators
// One generator per age segment and phase (if they share the same value and variance -> we create one object and use the same pointer for everyone
class coefficient_factory {
public:
    template<typename... Args>
    static coefficient* create_coefficient(std::string const &coefficient_id, Args&&... args) {
        if (coefficient_id == "fixed")
            return new fixed_coefficient(std::forward<Args>(args)...);
        else if (coefficient_id == "normal")
            return new normal_coefficient(std::forward<Args>(args)...);
        else throw std::out_of_range("Output delay buffer type not found");
    }
};
