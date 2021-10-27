#pragma once

#include "Instance.h"
#include "Solution.h"

struct IDecoder {

   const Instance &inst;

   IDecoder(const Instance &inst_): inst(inst_) {
      // Empty
   }

   virtual ~IDecoder() {
      // Empty
   }

   virtual const std::string decoderName() = 0;

   virtual int chromosomeLength() const = 0;

   virtual Solution decodeSolution(const std::vector <double> &chromosome) const = 0;

   double decode(const std::vector <double> &chromosome) const {
      auto sol = decodeSolution(chromosome);
      return sol.cachedCost;
   }

};
