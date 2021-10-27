cmake_minimum_required(VERSION 3.0)

# CPLEX-related configurations
# Configuration of CPLEX libraries.
set (CPLEX_PREFIX $ENV{CPX})
if (NOT CPLEX_PREFIX)
   set(CPLEX_PREFIX "/home/alberto/.local/ibm/ILOG/CPLEX_Studio201")
endif()
message(STATUS "Using CPLEX_PREFIX = \"${CPLEX_PREFIX}\"")

# Search for header files.
find_path (CPLEX_HEADER ilcplex/ilocplex.h ${CPLEX_PREFIX}/cplex/include)
find_path (CONCERT_HEADER ilconcert/iloenv.h ${CPLEX_PREFIX}/concert/include)

# And look for its libraries.
# You may need to change these paths according to your architecture.
find_library (CPLEX_LIB_CPLEX cplex "${CPLEX_PREFIX}/cplex/lib/x86-64_linux/static_pic/")
find_library (CPLEX_LIB_ILOCPLEX ilocplex "${CPLEX_PREFIX}/cplex/lib/x86-64_linux/static_pic/")
find_library (CPLEX_LIB_CONCERT concert "${CPLEX_PREFIX}/concert/lib/x86-64_linux/static_pic/")
set(CPLEX_INCLUDE_DIRS ${CPLEX_HEADER} ${CONCERT_HEADER})
set(CPLEX_LIBRARIES ${CPLEX_LIB_ILOCPLEX} ${CPLEX_LIB_CPLEX} ${CPLEX_LIB_CONCERT} -pthread -lgomp -ldl)

# Needed by GCC > 4.9 (and > 5.2 for `ignored-attributes`) to
# supress warnings related to CPLEX headers.
add_definitions(-Wno-deprecated-declarations -Wno-ignored-attributes -Wno-deprecated)

# Now, put the path of CPLEX header in compiler search path.
include_directories(SYSTEM ${CPLEX_INCLUDE_DIRS})
