//
// Created by Simon on 02/04/2018.
//

#include <vector>
#include "horn_day.h"

void HORN::DayMinimization(const std::vector <FCA::ImplicationInd> &L, std::vector <FCA::ImplicationInd> &Lc) {
    Lc.clear();

    if (L.empty()) { return; }

    Lc = L;
    size_t impl = Lc.size();
    size_t attr = Lc.front().Premise().size();
    FCA::ImplicationInd swap;
    FCA::ImplicationInd tmp;
    FCA::BitSet premise(attr), conclusion(attr);

    premise.reset();
    conclusion.reset();

    for(size_t i = 0; i < impl; ++i){
        tmp = Lc[i];
        Lc[i].Premise().reset();
        Lc[i].Conclusion().reset();

        FCA::LinClosure::Apply(tmp.Premise(), Lc, premise);
        FCA::LinClosure::Apply(tmp.Premise() |= tmp.Conclusion(), Lc, conclusion);

        Lc[i] = FCA::ImplicationInd(premise, conclusion);
    }

    int left = 0;
    int right = (int) Lc.size() - 1;

    while(left <= right){
        if(Lc[left].Premise() == Lc[left].Conclusion()) {
            swap = Lc[left];
            Lc[left] = Lc[right];
            Lc[right] = swap;
            right--;
        } else {
            left++;
        }
    }

    Lc.erase(Lc.begin() + left, Lc.end());

}
