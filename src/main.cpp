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

#include <fstream>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>
#include "../model/geographical_coupled.hpp"

using namespace std;
using namespace cadmium;
using namespace cadmium::celldevs;

using TIME = float;

/*************** Loggers *******************/
static ofstream out_messages("../logs/pandemic_outputs.txt");
struct oss_sink_messages{
    static ostream& sink(){
        return out_messages;
    }
};
static ofstream out_state("../logs/pandemic_state.txt");
struct oss_sink_state{
    static ostream& sink(){
        return out_state;
    }
};

using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;


int main(int argc, char ** argv) {
    if (argc < 2) {
        cout << "Program used with wrong parameters. The program must be invoked as follows:";
        cout << argv[0] << " SCENARIO_CONFIG.json [MAX_SIMULATION_TIME (default: 500)]" << endl;
        return -1;
    }

    try {
        
        // The C++ standard filesystem library is not used as it may require an additional linker flag (-std=c++17),
        // but more importantly that in certain versions of GCC the filesystem is contained in an experimental folder (GCC 7).
        // Newer versions of GCC doesn't have this problem (apparently GCC 8+ ?). As a result, depending on the version of GCC
        // used different code is required, so an older version of code to try to open a file is used.

        // A check to see if the file exists / can be accessed because the error message the JSON library gives if the
        // file does not exist is not informative (at the time of this writing).
        std::ifstream file_existence_checker{argv[1]};

        if(!file_existence_checker.is_open()) {
            throw std::runtime_error{"Unable to open the file: " + std::string{argv[1]}};
        }

        // Note: At the time of this writing, the web viewer that consumes the log files of this simulator relies on the
        // the input to geographical_coupled parameter (param name: id) to be empty; this changes how the IDs of cells
        // in the log files are printed.
        geographical_coupled<TIME> test = geographical_coupled<TIME>("");
        std::string scenario_config_file_path = argv[1];
        test.add_cells_json(scenario_config_file_path);
        test.couple_cells();

        std::shared_ptr<cadmium::dynamic::modeling::coupled < TIME>>
        t = std::make_shared<geographical_coupled<TIME>>(test);

        cadmium::dynamic::engine::runner <TIME, logger_top> r(t, {0});
        float sim_time = (argc > 2) ? atof(argv[2]) : 500;
        r.run_until(sim_time);
    }
    catch(std::exception &e) {
        // With cygwin, an exception that terminates the program may not be printed to the screen, making it unclear
        // if an error occurred. Thus an explicit print is done, along with a rethrowing of the exception to keep
        // the original termination logic the same.
        std::cerr << "A fatal error occurred: " << e.what() << std::endl;
        throw;
    }

    return 0;
}
