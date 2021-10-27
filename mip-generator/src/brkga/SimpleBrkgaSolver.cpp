
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include "SimpleBrkgaSolver.h"
#include "SortingDecoder.h"
#include "Timer.h"

#include <iostream>
#include <memory>

using namespace std;

SimpleBrkgaSolver::SimpleBrkgaSolver(const Instance &inst, const int rngSeed): inst(inst), rngSeed(rngSeed), bestSol(inst) {
   // Parameters from GECCO 2020 article, with a reduced number of generations.
   popSize = 885;
   maxGenerations = 600;
   pElite = 0.20665;
   pMutant = 0.05407;
   biasRho = 0.32728;

}

SimpleBrkgaSolver::~SimpleBrkgaSolver() {
   // Empty
}

void SimpleBrkgaSolver::solve(bool showProgress) {
   auto dec = std::make_unique<SortingDecoder>(inst);
   auto rng = std::make_unique<MTRand>(rngSeed);

   auto solver = std::make_unique<BRKGA<SortingDecoder, MTRand>>(
      dec->chromosomeLength(), 
      popSize,      
      pElite,       
      pMutant,      
      biasRho,      
      *dec,         
      *rng,          
      1,            
      omp_get_max_threads()
   );
  
   if (showProgress)
      cout << "Starting BRKGA." << endl;

   int generation = 0;
   Timer timer;
   timer.start();
   
   do {
      solver->evolve();
      ++generation;

      if (showProgress && (generation % 50 == 0 || generation == maxGenerations)) {
         timer.finish();
         cout << "Generation = " << generation
            << ", best solution =  " << solver->getBestFitness()
            << ", elapsed time = " << timer.elapsed() << " secs." << endl;
      }
   } while (generation < maxGenerations);
   
   timer.finish();
   bestSol = dec->decodeSolution(solver->getBestChromosome());
   solveTime = timer.elapsed();
   
   for (auto [key, task]: dec->decodeTaskOrder(solver->getBestChromosome())) {
      bestSol.taskOrder.push_back(task);
   }
   
   if (showProgress)
      cout << "BRKGA finished with solution value of " << bestSol.cachedCost << "." << endl;
}

const std::vector<Solution::Route> &SimpleBrkgaSolver::getRoutes() const {
   return bestSol.routes;
}

const std::vector <Task> &SimpleBrkgaSolver::getTaskAsgOrder() const {
   return bestSol.taskOrder;
}

const std::vector <int> SimpleBrkgaSolver::getNodeAsgOrder() const {
   vector <int> result;
   for (auto t: getTaskAsgOrder())
      result.push_back(t.node);
   return result;
}

