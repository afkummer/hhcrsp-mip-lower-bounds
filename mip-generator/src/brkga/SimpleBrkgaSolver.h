#pragma once

#include "Instance.h"
#include "Solution.h"

struct SimpleBrkgaSolver {
   SimpleBrkgaSolver(const Instance &inst, const int rngSeed = 1);
   
   virtual ~SimpleBrkgaSolver();
   
   void solve(bool showProgress = true);
   
   const std::vector<Solution::Route> &getRoutes() const;
   
   const std::vector <Task> &getTaskAsgOrder() const;
   const std::vector <int> getNodeAsgOrder() const;

   const Instance &inst;
   
   int rngSeed;
   int popSize;
   int maxGenerations;
   double pElite;
   double pMutant;
   double biasRho;

   Solution bestSol;
   double solveTime;
};

