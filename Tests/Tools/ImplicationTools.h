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
#include "../../CanonicalBasis/fca_datastructures.h"
#include "../../CanonicalBasis/fca_closure_operators.h"
#include "../../CanonicalBasis/fca_object_incremental.h"
#include "../../CanonicalBasis/fca_impec.h"
#include "../../CanonicalBasis/fca_mingen.h"
#include "../../CanonicalBasis/horn_minimization_algo.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

typedef std::vector<FCA::ImplicationInd> theory;

namespace ImplicationTools {

    void Reduce(theory &L);

    // read implication file
    void ReadFile(std::string filename, theory &L);

    // write implication file
    void WriteFile(std::string filename, const theory &L);

    // expand a theory with armstrong rules
    void ExpandTheory(theory &L, double growth);

    // generates random theory
    void GenerateTheory(theory &L, size_t attrNum, size_t implNum, bool closeempty=true, bool reduced=false);

    // read a string implication to a bit implication
    void ReadImplication(const std::string &line, const size_t attrNum, theory &L);

    FCA::ImplicationInd ArmstrongUnion(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongAugmentationP(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongAugmentationC(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongPTransitivity(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongReductionC(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    FCA::ImplicationInd ArmstrongReductionP(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B);

    // generates random bitset
    FCA::BitSet GetRandomBitSet(size_t attrNum);

    // generates random implication
    FCA::ImplicationInd GetRandomImplication(size_t attrNum);

    // close a set in a theory
    void CloseSet(const FCA::BitSet &X, theory &L);

    // read a context from a file
    void ReadContext(FCA::Context &c, std::string &filename);

    void ExportContextualTheory(const FCA::Context &c, std::string &name);
    
    // filename of a context and name of dataset
    void getRealBasis(std::string &filename, std::string &name);

    bool AreEquivalent(const theory &L1, const theory &L2);


};


#endif //ALGORITHMS_IMPLICATIONTOOLS_H
