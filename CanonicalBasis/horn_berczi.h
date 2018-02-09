//
// Created by Simon on 09/02/2018.
//

#ifndef ALGORITHMS_HORN_BERCZI_H
#define ALGORITHMS_HORN_BERCZI_H

#include <vector>
#include "fca_datastructures.h"
#include "fca_closure_operators.h"

namespace HORN
{

    std::vector<FCA::ImplicationInd> BercziMinimization(const std::vector<FCA::ImplicationInd> &L);

}


#endif //ALGORITHMS_HORN_BERCZI_H
