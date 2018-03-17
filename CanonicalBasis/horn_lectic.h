//
// Created by Simon on 16/03/2018.
//

#ifndef ALGORITHMS_HORN_LECTIC_H
#define ALGORITHMS_HORN_LECTIC_H

#include <vector>
#include "fca_implications.h"
#include "fca_linclosure.h"


namespace HORN {

    bool SizeComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2);

    bool InclusionComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2);

    void LecticMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc);


}

#endif //ALGORITHMS_HORN_LECTIC_H
