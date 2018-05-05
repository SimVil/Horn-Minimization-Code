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

    eq = (eq || I_1.Premise().test(i - 1));

    return eq;
}


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
            FCA::Closure::Apply(tmp.Conclusion(), Lc, tmp.Conclusion());
            Lm.push_back(tmp);
        }

        --i;

    }

    Lc = Lm;

}


void HORN::getQuasiClosed(const theory &L, theory &Lc) {
    Lc.clear();

    if (L.empty())
        return;

    FCA::ImplicationInd imp;
    Lc = L;

    size_t size = Lc.size() - 1;

    for (auto i = (int) size; i >= 0; --i)
    {
        imp = Lc[i];
        Lc[i] = Lc.back();
        Lc.pop_back();

        FCA::Closure::Apply(imp.Premise(), Lc, imp.Premise());
        if (!imp.Conclusion().is_subset_of(imp.Premise()))
        {
            imp.Conclusion() |= imp.Premise();
            Lc.emplace_back(FCA::ImplicationInd(imp.Premise(), imp.Conclusion()));
        }

    }
}
