//
// Created by Simon on 17/03/2018.
//

#include "horn_shock.h"

bool HORN::ReverseLecticComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2) {
    size_t size = I_1.Premise().size();
    size_t i = 0;
    bool eq; // = I_1.Premise().count() < I_2.Premise().count();

    do {
        eq = I_1.Premise().test(i) == I_2.Premise().test(i);
        i++;
    } while (i < size && eq);

    eq = (eq || I_1.Premise().test(i - 1));

    return eq;
}


void HORN::ShockMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc) {
    Lc.clear();

    if (L.empty())
        return;

    FCA::BitSet pclosure, cclosure;
    FCA::ImplicationInd imp;
    Lc = L;

    std::sort(Lc.begin(), Lc.end(), ReverseLecticComp);
    size_t size = Lc.size();

    for (size_t i = 0; i < size; ++i)
    {
        imp = Lc.back();
        Lc.pop_back();
        pclosure.reset();
        cclosure.reset();

        FCA::LinClosure::Apply(imp.Premise(), Lc, pclosure);
        if (!imp.Conclusion().is_subset_of(pclosure))
        {
            FCA::LinClosure::Apply(imp.Conclusion(), Lc, cclosure);
            Lc.insert(Lc.begin(), FCA::ImplicationInd(imp.Premise(), cclosure));
        }

    }
}
