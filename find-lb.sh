#!/bin/bash

# Check command line args.
if [ $# -ne 3 ]; then
   echo "Usage: $0 <1:instance path> <2:scenario id> <3:time limit>"
   exit 1
fi

INST="$(realpath $1)"
SCE=$2
TILIM=$3

echo "-------------------------"
echo "--- MIP-LB with CPLEX ---"
echo "-------------------------"
echo "Instance file: $INST"
echo "Scenario ID: $SCE"
echo "Solver time limit: $TILIM sec."
echo 

# Components that set the scenarios.
RELAX_MODEL=0
LIMIT_THREADS=0
MEM_EMPH=0
WARMSTART=0
RCUTS=0

# Detects the scenario.
echo "Scenario description:"
case "${SCE}" in

   "1")
      RELAX_MODEL=1
      echo "- Solving linear programming relaxation."
   ;;

   "2")
      # Empty
      echo "- Using default CPLEX settings."
   ;;

   "3")
      MEM_EMPH=1
      echo "- Memory emphasis flag set ON."
   ;;

   "4")
      LIMIT_THREADS=1
      echo "- Setting a limit of 1 thread."
   ;;

   "5")
      MEM_EMPH=1
      LIMIT_THREADS=1
      echo "- Memory emphasis flag set ON."
      echo "- Setting a limit of 1 thread."
   ;;

   "6")
      MEM_EMPH=1
      LIMIT_THREADS=1
      WARMSTART=1
      echo "- Memory emphasis flag set ON."
      echo "- Setting a limit of 1 thread."
      echo "- Setting MIP warmstart solution."
   ;;

   "7")
      MEM_EMPH=1
      LIMIT_THREADS=1
      RCUTS=1
      echo "- Memory emphasis flag set ON."
      echo "- Setting a limit of 1 thread."
      echo "- Generating additional/redundant cuts."
   ;;

   *)
      echo "Unknown scenario '$SCE'."
      echo "Exiting."
      exit 1
   ;;
esac

echo ""

# Creates the working directory for the run.
WDIR="$(mktemp -p . -d run.s${SCE}.XXXXXXXX)"
echo "Work dir: $WDIR"

# Generates CPLEX stdin.
CPX_STDIN="${WDIR}/cpx-stdin"

# First step: common commands for all scenarios.
echo "set randomseed 1" > $CPX_STDIN
echo "set time $TILIM" >> $CPX_STDIN
echo "read model.lp" >> $CPX_STDIN

if [ $RELAX_MODEL -eq 1 ]; then
   echo "change problem lp" >> $CPX_STDIN
fi

if [ $LIMIT_THREADS -ne 0 ]; then
   echo "set thread 1" >> $CPX_STDIN
fi

if [ $MEM_EMPH -eq 1 ]; then
   echo "set emp mem y" >> $CPX_STDIN
fi

if [ $WARMSTART -eq 1 ]; then
   echo "read initial.sol" >> $CPX_STDIN
fi

# Last step: write common 'ending' commands
echo "opt" >> $CPX_STDIN
echo "write final.sol" >> $CPX_STDIN
echo "quit" >> $CPX_STDIN
echo "" >> $CPX_STDIN

# Enters the workdir to create the model.
cd "${WDIR}"
export RCUTS
SKIP_OPT=1 ../mip-generator/build/exactMip $INST 9999 1 $WARMSTART | tee exactMip.log

echo
echo
echo "Invoking CPLEX..."
cplex < "cpx-stdin"
