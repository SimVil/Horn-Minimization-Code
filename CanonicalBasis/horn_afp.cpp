//
// Created by Simon on 21/03/2018.
//

#include "horn_afp.h"


void HORN::AFPMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc) {
    Lc.clear();

    if (L.empty())
        return;

    size_t attrNum = L.front().Premise().size();
    size_t implNum = L.size();

    std::list<FCA::BitSet> S;
    FCA::BitSet premise(attrNum), Lc_closure(attrNum), L_closure;
    FCA::BitSet C(attrNum), D(attrNum), M(attrNum);
    FCA::ImplicationInd copy;
    bool found;
    M.set();

    for (auto &impL : L){
        S.clear();
        S.emplace_back(impL.Premise());

        do{
            premise = S.back();
            S.pop_back();
            FCA::LinClosure::Apply(premise, Lc, Lc_closure);
            FCA::LinClosure::Apply(Lc_closure, L, L_closure);

            if(Lc_closure != L_closure){
                found = false;

                for (auto &impLc : Lc){
                    C = impLc.Premise();
                    C &= Lc_closure;  // C := A n X


                    if (C != impLc.Premise()){
                        FCA::LinClosure::Apply(C, L, D);
                        if (C != D){
                            found = true;
                            copy = impLc;
                            impLc = FCA::ImplicationInd(C, D);   // replacement in K
                            Lc_closure |= D;
                            S.emplace_back(Lc_closure);

                            if (copy.Conclusion() != D){
                                S.emplace_back(copy.Premise());
                            }
                        }
                    }

                }

                if (!found) { Lc.emplace_back(FCA::ImplicationInd(Lc_closure, M)); }

            }

        } while (!S.empty());

    }

}
