Cell-DEVS-GeographyBased
===


Overview
----

A modified implementation of the Zhong model. The original model that this implementation is based off of can be obtained at:


https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7088542/pdf/11430_2009_Article_44.pdf

This implementation adds additional features to more closely model the real world. See the documentation for more information.

Documentation
----

See the Documentation.docx file located at the root of this repository.

Compiling the Program
---

Note: A C++17 compliant compiler is required, along with the Cadmium library (available at: https://github.com/SimulationEverywhere/cadmium/tree/celldevs-json)

**Using the Terminal**

1. Clone this repository and navigate to the source code

2. Enter the following on the command line:

   gcc -o ZhongExecutable -I _location_to_cadmium_/cadmium-json/include -I _location_to_cadmium_/cadmium-json/json/include -std=c++17 ./model/cells/zhong_cell.hpp ./model/zhong_coupled.hpp ./src/main.cpp -lstdc++ -lm

----

Running a Model
----

To run the model:

1. Go to parent directory with the compiled executable
2. Create a directory called logs (if one does not already exist)
3. Navigate to the directory with the executable and enter the following command:
   ./named_of_executable location_to_configuration_file.json run_until_this_many_simulation_cycles
   
   The last paramter is optional. The default is 500 simulation cycle.
