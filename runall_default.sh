## Runs all utilities of the Geography-Based-Model
## Written by Glenn
## The script assumes the model is compiled and the environment running this script includes python and python geopandas

## defining commands used
SIMULATE="./pandemic-geographical_model ../config/scenario.json 50"
PARSE_MSG_LOGS="java -jar sim.converter.glenn.jar "input" "output""

## defining directories used
VISUALIZATION_DIR="GIS_Viewer/ottawa/simulation_runs/run1"

## make directories if they don't exist
mkdir -p Scripts/Input_Generator/output
mkdir -p Scripts/Msg_Log_Parser/input
mkdir -p Scripts/Msg_Log_Parser/output
mkdir -p ${VISUALIZATION_DIR}

## generate a scenario json file for model input, save it in the config folder
cd Scripts/Input_Generator
python generate_cadmium_json_g2.py
cp output/scenario.json ../../config

## run the model
cd ../../bin
echo
echo "Executing model:"
echo ${SIMULATE}
${SIMULATE}

## generate SIRDS graphs
echo
echo "Generating graphs (will be found in logs folder):"
cd ../Scripts/Graph_Generator/
python graph_generator.py
cd ../..

## Copy the message log + scenario to message log parser's input
# Note this deletes the contents of input/output folders of the message log parser before executing
echo
echo "Copying simulation results to message log parser:"

rm -f Scripts/Msg_Log_Parser/input/*
rm -rf "Scripts/Msg_Log_Parser/output\pandemic_messages"
rm -f Scripts/Msg_Log_Parser/*.zip
rm -f Scripts/Msg_Log_Parser/output/*
cp config/scenario.json Scripts/Msg_Log_Parser/input
cp logs/pandemic_outputs.txt Scripts/Msg_Log_Parser/input

# rename the log file to *messages.txt
mv Scripts/Msg_Log_Parser/input/pandemic_outputs.txt Scripts/Msg_Log_Parser/input/pandemic_messages.txt

# Run the message log parser
echo
echo "Running message log parser:"
cd Scripts/Msg_Log_Parser
echo ${PARSE_MSG_LOGS}
${PARSE_MSG_LOGS}
echo
unzip "output\pandemic_messages.zip" -d output

## Copy the converted message logs to GIS Web Viewer Folder
echo
echo "Copying converted files to: ${VISUALIZATION_DIR}"
cd ../..
cp Scripts/Msg_Log_Parser/output/messages.log ${VISUALIZATION_DIR}
cp Scripts/Msg_Log_Parser/output/structure.json ${VISUALIZATION_DIR}
