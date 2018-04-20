//
// Created by Simon on 20/04/2018.
//

#include "horn_duquenne.h"


bool HORN::LecticComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2) {
    size_t size = I_1.Premise().size();
    size_t i = 0;
    bool eq; // = I_1.Premise().count() < I_2.Premise().count();

    do {
        eq = I_1.Premise().test(i) == I_2.Premise().test(i);
        i++;
    } while (i < size && eq);

    eq = (eq || !I_1.Premise().test(i - 1));

    return eq;
}


void HORN::DuquenneMinimization(const theory &L, theory &Lc) {

    getQuasiClosed(L, Lc); // Lc contains quasi-closure on the left
    std::sort(Lc.begin(), Lc.end(), LecticComp);

    theory Lm;
    FCA::ImplicationInd tmp;
    size_t left = 0;
    size_t right = Lc.size() - 1;
    size_t i = 0;
    bool redundant = false;

    while(left <= right){

        i = 0;
        redundant = false;
        while(i < Lm.size() && !redundant){
            if (Lm[i].Premise().is_subset_of(Lc[left].Premise()) &&
                    !Lm[i].Conclusion().is_subset_of(Lc[left].Premise())){

                tmp = Lc[right];
                Lc[right] = Lc[left];
                Lc[left] = tmp;
                right --;
                redundant = true;

            }
        }

        if (!redundant){
            tmp = Lc[left];
            FCA::LinClosure::Apply(Lc[left].Conclusion(), Lc, tmp.Conclusion());
            Lm.emplace_back(tmp);
            left ++;

        }
    }

    Lc.erase(Lc.begin() + left, Lc.end());

}


void HORN::getQuasiClosed(const theory L, theory &Lc) {
    Lc.clear();

    if (L.empty())
        return;

    FCA::BitSet pclosure, cclosure;
    FCA::ImplicationInd imp;
    Lc = L;

    size_t size = Lc.size();

    for (size_t i = 0; i < size; ++i)
    {
        imp = Lc[i];
        Lc[i].Premise().reset();
        Lc[i].Conclusion().reset();

        pclosure.reset();
        cclosure.reset();

        FCA::LinClosure::Apply(imp.Premise(), Lc, pclosure);
        if (!imp.Conclusion().is_subset_of(pclosure))
        {
            imp.Conclusion() |= imp.Premise();
            Lc[i] =  FCA::ImplicationInd(imp.Premise(), imp.Conclusion();
        }

    }
}
