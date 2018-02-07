//
// Created by Simon on 06/02/2018.
//

#include "fca_closure_operators.h"
#include "fca_datastructures.h"
#include <vector>
#include "horn_maier.h"

std::vector<FCA::BitSet> getEquivalenceClasses(const std::vector<FCA::BitSet> &matrix)
{

}

void HORN::getEquivalenceMatrix(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::BitSet> &matrix)
{
    if (L.empty())
        return;

    unsigned int implNum = L.size();
    unsigned int attrNum = L.front().Premise().size();
    matrix.resize(implNum);

    FCA::BitSet tmp(attrNum);
    tmp.reset();

    for(int i = 0; i < implNum; ++i)
    {
        matrix[i].resize(attrNum);
        matrix[i].reset();

        FCA::LinClosure::Apply(L[i].Premise(), L, tmp, 0, &matrix[i]);
    }

}


std::vector<FCA::ImplicationInd> HORN::redundancyElimination(const std::vector<FCA::ImplicationInd> &L) {
    return std::vector<FCA::ImplicationInd>();
}

std::vector<FCA::ImplicationInd> HORN::MaierMinimization(const std::vector<FCA::ImplicationInd> &L) {
    return std::vector<FCA::ImplicationInd>();
}