default.json - Is the default cell and simulation config, used as an input to the scenario generator

fields.json - Describes the format of the state object written to logs. It is inserted outside of "cells", it ised used by the GIS web viewer v2

infectedCell.json - Describes a cell ID and infected state object where the infection should begin. The scenario json is first generated with each cell having the default.json values, and then the infected cell's state object is overwritten with the values in infectedCell.json
Currently one one cell in this file is supported, and this file must exist for the post processing to work.
