#pragma once

#define IL_STD
#include <ilcplex/ilocplex.h>

#include <boost/multi_array.hpp>

#include <random>

class Instance;

class MipModel {
public:
   MipModel(const Instance &inst);
   virtual ~MipModel();

   void buildModel();
  
   void writeLp(const char *fname);
   void writeSolution(const std::string &fname) const;

   int numCols() const;
   int numRows() const;
   long numNonZeros() const;

   void maxThreads(int value);
   void timeLimit(int maxSeconds);

   void solve();
   double solveSeconds() const;
   void printCostSummary() const;

   double objValue() const;
   double gap() const; // Relative, not percentual.
   double lb() const;

   double computeDist() const;
   double computeTard() const;
   double tmax() const;

   void setBoundX(int i, int j, int v, int s, double lb = 0.0, double ub = 1.0);
   void resetBoundsX();

   // get<0> -> vehicle ID
   // get<1> -> service start time
   std::tuple <int,double> svcStartTime(int i, int s);

   double assignment(int i, int j, int v, int s);

   // [route position]
   // get<0> -> node
   // get<1> -> skill
   std::vector <std::tuple<int, int>> extractRoute(int v) const;

   // [vehicle][route position]
   // get<0> -> node
   // get<1> -> skill
   std::vector <std::vector <std::tuple<int, int>>> extractRoutes() const;

   void loadRoutes(const std::vector<std::vector<std::tuple<int, int>>> &r);

protected:
   const Instance &m_inst;

   IloEnv m_env;
   IloModel m_model;
   IloCplex m_cplex;
   IloObjective m_obj;
   IloExpr m_expr;

   IloNumVarArray m_xSeq;
   IloNumVar m_tmax;

   boost::multi_array<IloNumVar, 4> m_x;
   boost::multi_array<IloNumVar, 3> m_t;
   boost::multi_array<IloNumVar, 2> m_z;

   double m_solveTime {0.0};

   void createVarsAndObj();
   void createAllConstraints();

   /*
    * Check if the flow constraint created by this pair of (node, vehicle)
    * as at least one input arc, and one output arc. If such is the case,
    * returns true and then the model can create the variables regarding
    * this node. Otherwise returns false and no variables must be created.
    */
   bool isFlowOk(int i, int v) const;
};
