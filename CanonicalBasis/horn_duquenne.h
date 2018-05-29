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

    template<typename ClosureOperator>
    void DuquenneMinimization(const theory &L, theory &Lc);

    template<>
    void DuquenneMinimization<FCA::LinClosure>(const theory &L, theory &Lc);

    void getQuasiClosed(const theory &L, theory &Lc);

}


// instanciation of ClosureOperator template for Duquenne
template <typename ClosureOperator>
void HORN::DuquenneMinimization(const theory &L, theory &Lc) {

    getQuasiClosed(L, Lc); // Lc contains quasi-closure on the left
    std::sort(Lc.begin(), Lc.end(), LecticComp);

    theory Lm;
    FCA::ImplicationInd tmp;
    size_t size = Lc.size() - 1;
    size_t j = 0;
    auto i = (int) size;
    bool redundant;

    while(i >= 0){
        j = 0;
        redundant = false;
        tmp = Lc[i];

        while(j < Lm.size() && !redundant){

            if (Lm[j].Premise().is_subset_of(tmp.Premise()) &&
                !Lm[j].Conclusion().is_subset_of(tmp.Premise())){

                Lc[i] = Lc.back();
                Lc.pop_back();
                redundant = true;
            }
            ++j;
        }

        if(!redundant){
            ClosureOperator::Apply(tmp.Conclusion(), Lc, tmp.Conclusion());
            Lm.push_back(tmp);
        }

        --i;

    }

    Lc = Lm;

}

#endif //ALGORITHMS_HORN_DUQUENNE_H
