//
// Created by Simon on 17/03/2018.
//

#include "horn_shock.h"

void HORN::ShockMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc) {
    Lc.clear();

    if (L.empty())
        return;

    FCA::BitSet pclosure, cclosure;
    FCA::ImplicationInd imp;
    Lc = L;

    size_t size = Lc.size();

    for (size_t i = 0; i < size; ++i)
    {
        imp = Lc.back();
        Lc.pop_back();

        FCA::LinClosure::Apply(imp.Premise(), Lc, pclosure);
        if (!imp.Conclusion().is_subset_of(pclosure))
        {
            FCA::LinClosure::Apply(imp.Conclusion(), Lc, cclosure);
            Lc.insert(Lc.begin(), FCA::ImplicationInd(imp.Premise(), cclosure));
        }

    }
}
