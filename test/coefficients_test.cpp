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

#include <boost/test/unit_test.hpp>
#include <vector>
#include "../model/cells/distributions/coefficient.hpp"


BOOST_AUTO_TEST_CASE(fixed) {
    double value = 1;
    auto generator = coefficient_factory::create_coefficient("fixed", value);
    for (int i = 0; i < 5; i++)
        BOOST_CHECK_EQUAL(generator->get_coefficient(), value);
}

BOOST_AUTO_TEST_CASE(normal) {
    double value = 0.5;
    double stddev = 0.5;
    double seed = 2345;
    int n = 100;
    // First we create a generator and get 100 outputs
    auto generator = coefficient_factory::create_coefficient("normal", value, stddev, seed);
    std::vector<double> vector = std::vector<double>();
    vector.reserve(n);
    for (int i = 0; i < n; i++)
        vector.push_back(generator->get_coefficient());
    // Then we create another generator with the same seed and check that the output is the same
    generator = coefficient_factory::create_coefficient("normal", value, stddev, seed);
    for (int i = 0; i < n; i++)
        BOOST_CHECK_EQUAL(generator->get_coefficient(), vector.at(i));
}