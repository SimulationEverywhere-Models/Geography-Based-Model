Cell-DEVS-GeographyBased
===

This is Glenn testing branch creation before pushing changes

Overview
----

A modified implementation of the Zhong model. The original model that this implementation is based off of can be obtained at:


https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7088542/pdf/11430_2009_Article_44.pdf

This implementation adds additional features to more closely model the real world. See the documentation for more information.

Documentation
----

See the `Manual.docx` file located at the root of this repository.

Requirements
---

* A C++17 compiler (GCC was used for development)

* A copy of this repository

* A copy of Cadmium with JSON input support, available at https://github.com/SimulationEverywhere/cadmium/tree/celldevs-json

  To get the aforementioned copy of cadmium with JSON support, the following commands can be used:
  
  1. git clone https://github.com/SimulationEverywhere/cadmium.git --single-branch --branch celldevs-json
  
  2. cd cadmium
  
  3. git submodule update --init --recursive
  
  **The above commands will change once the branched clone from, celldevs-json, is merged into master in its repository.
  Afterwards, the first command should be replaced with `git clone https://github.com/SimulationEverywhere/cadmium.git`**

  The dependencies for cadmium must also be installed. Refer to the document located at:
   http://www.sce.carleton.ca/courses/sysc-5104/lib/exe/fetch.php?media=cadmium.pdf

  The location of cadmium must be in the parent folder that holds this project.
  For example, if the location of this cloned repository has the location of parentFolder/GeographicalModel, then the cadmium library
  must be located in parentFolder.

Compiling the Program
---

**Using CMAKE**

1. Navigate to the root directory of the copy of this project

2. Enter the following command `cmake CMakeLists.txt`

3. Execute the make file using the command: make

----

Running a Model
----

To run the model:

1. Create a directory called logs in the root directory of this project (if one does not already exist)

2. Go to the bin folder from the root directory

3. Enter the following command:
   `./pandemic-geographical_model) <location_to_configuration_file.json> [<simulation time>]`
   
The last parameter is optional. The default is 500.
   
