//
// Created by Simon on 21/03/2018.
//

#ifndef ALGORITHMS_HORN_AFP_H
#define ALGORITHMS_HORN_AFP_H

#include "fca_linclosure.h"
#include "fca_implications.h"
#include <list>
#include <deque>
#include <map>

namespace HORN {

    void AFPMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc);

}


#endif //ALGORITHMS_HORN_AFP_H
