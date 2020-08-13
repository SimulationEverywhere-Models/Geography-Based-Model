Description of File(s) In This Folder
===

1. **`simulation_config.hpp`**:

Holds the parameters that control the simulation. In particular:

* The virulence rates
* The recovery rates
* The mobility rates
* The fatality rates

2. **`sir.hpp`**:

Holds the state of each cell in the simulation. The states of each cell are updated
throughout the simulation, and provides the functionality to print the state information
of each cell to the simulation log file (the location of the log file is defined in `main.cpp`).

The state information stored is:

* Proportion of each population that fits within the age groups used in the simulation
* The proportion of susceptible population for each age group
* The proportion of each age group at each infected stage
* The proportion of each age group at each recovered stage
* The proportion of each age group that are fatalities of the pandemic

3. **`vicinity.hpp`**:

Holds the correlation between two cells. Every neighbor of a cell has an instance
of this structure. Thus for any given cell, the correlation for all surrounding neighbors
can be found (this is implemented in the `zhong_cell.hpp`).

4. **`zhong_cell.hpp`**:

Holds the implementation of the model that runs different simulations. It uses all of the
aforementioned structures to run simulations. This implementation is described in the
associated user guide, located at the root of the repository.
