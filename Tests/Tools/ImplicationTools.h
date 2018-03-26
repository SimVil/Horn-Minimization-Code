//
// Created by Simon on 22/03/2018.
//

#ifndef ALGORITHMS_IMPLICATIONTOOLS_H
#define ALGORITHMS_IMPLICATIONTOOLS_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <cassert>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include "../../CanonicalBasis/fca_implications.h"
#include "../../CanonicalBasis/fca_linclosure.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

typedef std::vector<FCA::ImplicationInd> theory;

namespace ImplicationTools {

    void Reduce(theory &L);

    void ReadFile(std::string filename, theory &L);

    void WriteFile(std::string filename, const theory &L);

    void ExpandTheory(theory &L, double growth);

    void GenerateTheory(theory &L, size_t attrNum, size_t implNum, bool closeempty=true);

    void ReadImplication(const std::string &line, const size_t attrNum, theory &L);

    FCA::ImplicationInd ArmstrongUnion(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongAugmentationP(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongAugmentationC(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongPTransitivity(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongReductionC(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongReductionP(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::BitSet GetRandomBitSet(size_t attrNum);

    FCA::ImplicationInd GetRandomImplication(size_t attrNum);

    void CloseSet(const FCA::BitSet &X, theory &L);


};


#endif //ALGORITHMS_IMPLICATIONTOOLS_H
