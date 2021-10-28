# MIP-based lower bounds for the home health care routing and scheduling problem

This repository contains the source code to generate `.lp` files for the HHCRSP. More details are available in my Ph.D. thesis at [LUME](https://www.lume.ufrgs.br/?locale-attribute=en).

To compile the project, you need a reasonably recent CPLEX version and the following extra dependencies.

- Boost multi-array library >= 1.60
- CMake >= 3.0

To build the application, open a terminal emulator and browse to (mip-generator/build)[mip-generator/build] subdirectory. Then, issue the command `cmake .. -DCPLEX_PREFIX=xxxx` replacing `xxxx` by the CPLEX install location. For example, one can run `cmake .. -DCMAKE_PREFIX=/opt/ibm/ILOG/CPLEX_Studio201`. 

Once compiled, the user can run experiments through the [find-lb.sh](find-lb.sh) script. 
