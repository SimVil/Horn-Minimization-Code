//
// Created by Simon on 20/04/2018.
//

#ifndef ALGORITHMS_HORN_DUQUENNE_H
#define ALGORITHMS_HORN_DUQUENNE_H

#include "fca_closure_operators.h"
#include "fca_implications.h"

typedef std::vector<FCA::ImplicationInd> theory;

namespace HORN {

    bool LecticComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2);

    void DuquenneMinimization(const theory &L, theory &Lc);

    void getQuasiClosed(const theory L, theory &Lc);

}


#endif //ALGORITHMS_HORN_DUQUENNE_H
