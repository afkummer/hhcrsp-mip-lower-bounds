/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020
 * Alberto Francisco Kummer Neto (afkneto@inf.ufrgs.br),
 * Luciana Salete Buriol (buriol@inf.ufrgs.br) and
 * Olinto César Bassi de Araújo (olinto@ctism.ufsm.br)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include "IDecoder.h"
#include "MipModel.h"

//#define SORT_DEC_ACCT

struct SortingDecoder: public IDecoder {

   using TElem = std::tuple<double, Task>;

   // Cache of vehicles by skills.
   // [skill id] -> vehicles list
   std::vector <std::vector <int>> vehiSkills;

   // Caches the task vector used into the decoding process.
   std::vector <TElem> initialTasks;

   SortingDecoder(const Instance &inst_);

   virtual ~SortingDecoder();
   
   virtual const std::string decoderName() override {
      return "SortingDecoder";
   }

   virtual int chromosomeLength() const override;

   virtual Solution decodeSolution(const std::vector <double> &chromosome) const override;

   std::vector <TElem> decodeTaskOrder(const std::vector <double> &chromosome) const;

#ifdef SORT_DEC_ACCT
   static long totalOperations();
   static long individualsDecoded();
#endif
};
