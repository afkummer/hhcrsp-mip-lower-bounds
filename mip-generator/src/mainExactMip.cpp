#include "Instance.h"
#include "MipModel.h"
#include "MemUsage.h"
#include "brkga/SimpleBrkgaSolver.h"
#include "brkga/SortingDecoder.h"

#include <algorithm>
#include <memory>

using namespace std;

int main(int argc, char **argv) {
   if (argc != 5) {
      std::cout << "Usage: " << argv[0] << " <1:instance path> <2:time limit> <3:num threads> <4:use initial solution>\n";
      return EXIT_FAILURE;
   }

   const char *instPath = argv[1];
   const int tilim = stoi(argv[2]);
   const int threads = stoi(argv[3]);
   bool initialSol = stoi(argv[4]) != 0;

   cout << "#######################################" << endl;
   cout << "### Exact MIP solver for the HHCRSP ###" << endl;
   cout << "#######################################" << endl;
   cout << "-- Instance file: " << instPath << endl;
   cout << "-- Max threads: " << threads << endl;
   cout << "-- Time limit: " << tilim << " seconds" << endl;
   cout << "-- Use initial solution: " << (initialSol ? "Yes (BRKGA)" : "No") << endl << endl;

   unique_ptr<Instance> inst(new Instance(argv[1]));
   unique_ptr<MipModel> mip(new MipModel(*inst));

   mip->buildModel();
   mip->writeLp("model.lp");
   cout << "Model exported to 'model.lp'." << endl;
   cout << "Memory usage: " << memUsage() << " MB" << endl;

   mip->maxThreads(threads);
   mip->timeLimit(tilim);

   if (initialSol) {
      cout << "\nCreating initial solution with BRKGA." << endl;
      auto brkga = SimpleBrkgaSolver(*inst);
      brkga.solve();
      mip->loadRoutes(brkga.getRoutes());

      cout << "\nSolving mip warm-start with constructive solution." << endl;
      mip->solve();
      mip->writeSolution("initial.sol");
      cout << "Warmstart solution written to 'initial.sol'." << endl;
   }

   if (auto env = getenv("SKIP_OPT"); env && env[0] == '1') {
      cout << "SKIP_OPT flag set. Exiting.\n";
      return EXIT_SUCCESS;
   } else {
      cout << "\nContinuing optimization through C++ code." << endl;
   }

   mip->resetBoundsX();
   mip->solve();

   const double bestInt = mip->objValue();
   const double mipGap = mip->gap();
   const double bestBound = mip->lb();

   const double cpuTime = mip->solveSeconds();

   int numCols = mip->numCols();
   int numRows = mip->numRows();
   long numNzz = mip->numNonZeros();

   ofstream fid("results-exactMip.csv", ios::out | ios::app);
   if (fid.tellp() == 0) {
      fid <<
         "instance," <<
         "num_cols," <<
         "num_rows," <<
         "num_nzz," <<
         "lb," <<
         "obj," <<
         "gap," <<
         "time," <<
         "dist," <<
         "tard," <<
         "tmax," <<
         "memusage" <<
         endl;
   }

   fid <<
      instPath << ',' <<
      numCols << ',' <<
      numRows << ',' <<
      numNzz << ',' <<
      bestBound << ',' <<
      bestInt << ',' <<
      mipGap << ',' <<
      cpuTime << "," <<
      mip->computeDist() << ',' <<
      mip->computeTard() << ',' <<
      mip->tmax() << ',' <<
      memUsage() <<
      endl;

   mip->printCostSummary();

   return EXIT_SUCCESS;
}
