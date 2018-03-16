//
// Created by Simon on 16/03/2018.
//

#include "horn_lectic.h"


bool HORN::LecticComp(FCA::ImplicationInd I_1, FCA::ImplicationInd I_2) {
    size_t size = I_1.Premise().size();
    size_t i = 0;
    bool eq;

    do{
        eq = I_1.Premise().test(i) == I_2.Premise().test(i);
        i++;
    } while(i < size && eq);

    return (eq || !I_1.Premise().test(i - 1));
}

void HORN::LecticMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc) {
    if (L.empty())
        Lc.clear();

    std::vector<FCA::ImplicationInd> Lcopy = L;
    FCA::BitSet Lclosure, LCclosure;

    std::sort(Lcopy.begin(), Lcopy.end(), LecticComp);
    for(auto &imp : Lcopy){
        FCA::LinClosure::Apply(imp.Premise(), L, Lclosure);
        FCA::LinClosure::Apply(imp.Premise(), Lc, LCclosure);

        if (Lclosure != LCclosure)
            Lc.emplace_back(FCA::ImplicationInd(LCclosure, Lclosure));
    }
}
