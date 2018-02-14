//
// Created by Simon on 09/02/2018.
//

#include "BercziTester.h"

void BercziTester::BuildingSequence(std::vector<std::vector<FCA::Implication>> &buff,
                                    const std::vector<FCA::ImplicationInd> &L,
                                    const std::vector<FCA::Attribute> &sigma)
{
    std::vector<FCA::ImplicationInd> tmp = HORN::BercziMinimization(L);
    buff.emplace_back(FCA::Convert(tmp, sigma));
}
