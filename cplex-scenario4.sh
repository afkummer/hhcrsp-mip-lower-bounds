#!/usr/bin/env bash

echo "--------------------------------------"
echo "--- MIP-LB Experiment - Scenario 4 ---"
echo "--------------------------------------"

if [ $# -ne 2 ]; then
   echo "Usage: $0 <1:path to instance> <2:tilim>"
   exit 1
fi

INST=$1
TILIM=$2
WDIR="$(mktemp -d -p . sce4.XXXXXXXX)"
GEN=/home/alberto/work/doct-ufrgs-thesis/mankowska2014/build/exactMip
export SKIP_OPT=1

echo "Instance: $INST"
echo "Time limit: $TILIM"
echo "Working directory: $WDIR"
echo "Model generator: $GEN"

printf "\n\n"
echo "(1/2) Generating model..."
$GEN $INST 9999 4 0 | tee ${WDIR}/exactMip.log
mv model.lp ${WDIR}/

printf "\n\n"
echo "(2/2) Invoking CPLEX..."
/home/alberto/.local/bin/cplex -c \
   "set randomseed 1" \
   "set time $TILIM" \
   "read ${WDIR}/model.lp" \
   "set threads 1" \
   "mipopt" \
   "write ${WDIR}/final.sol" \
   "quit" \
   "" | tee ${WDIR}/cplex.log

rm cplex.log 
echo "Done!"
