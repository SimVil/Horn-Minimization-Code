//
// Created by Simon on 02/04/2018.
//

#ifndef ALGORITHMS_HORN_DAY_H
#define ALGORITHMS_HORN_DAY_H

#include "fca_linclosure.h"
#include "fca_implications.h"

namespace HORN {

    void DayMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc);
}

#endif //ALGORITHMS_HORN_DAY_H
