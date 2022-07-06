## Scenario Generation

the script generate_cadmium_json_g2.py will create a scenario.json for the Geography-Based-Model (SIRDS) for the Cadmium master branch (as of Feb 20 2021)
the inputs to the process are found in the input folder, the output scenario from this process will be located in the output folder

Requirements before running:
- the python environment must have geopandas installed before running generateScenario.sh

Inputs/Outputs:
- the default cell state can be set in input/default.json
- the infected cell can be set in input/infectedCell.json
- input/fields.json inserts information for message log parsing to be used with GIS Web viewer v2

