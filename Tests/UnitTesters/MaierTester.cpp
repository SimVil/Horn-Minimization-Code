//
// Created by Simon on 09/02/2018.
//

#include "MaierTester.h"


void MaierTester::BuildingSequence(std::vector<std::vector<FCA::Implication>> &buff,
                                   const std::vector<FCA::ImplicationInd> &L,
                                   const std::vector<FCA::Attribute> &sigma) {
    std::vector<FCA::ImplicationInd> tmp;

    tmp = HORN::redundancyElimination(L);
    buff.emplace_back(FCA::Convert(tmp, sigma));

    tmp = HORN::MaierMinimization(L);
    buff.emplace_back(FCA::Convert(tmp, sigma));
}
