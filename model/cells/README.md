** Description of File(s) In This Folder **

simulation_configuration:

Holds the parameters that control the simulation. In particular:

* The virulence rates
* The recovery rates
* The mobility rates
* The fatality rates

sir:

Holds the state of each cell in the simulation. The states of each cell are updated
throughout the simulation, and provides the functionality to print the state information
of each cell to the simulation log file (the location of the log file is defined in main.cpp).

The state information stored is:

* Proporiton of each population that fits within the age groups used in the simulation
* The proportion of susceptible population for each age group
* The proporition of each age group at each infected stage
* The proportion of each age group at each recovered stage
* The proportion of each age group that are fatalities of the pandemic

vicinity:

Holds the correlation between two cells. Every neighbour of a cell has an instance
of this structure. Thus for any given cell, the correlation for all surrounding neighbours
can be found (this is implemented in the zhong_cell.hpp).

zhong_cell:

Holds the implementation of the model that runs different simulations. It uses all of the
aforementioned structures to run simulations. This implementation is described in the
 associated user guide, located at.