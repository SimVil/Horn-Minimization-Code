//
// Created by Simon on 17/03/2018.
//

#ifndef ALGORITHMS_HORN_SHOCK_H
#define ALGORITHMS_HORN_SHOCK_H

#include "fca_implications.h"
#include "fca_linclosure.h"

namespace HORN {

    bool ReverseLecticComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2);

    void ShockMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc);
}


#endif //ALGORITHMS_HORN_SHOCK_H
