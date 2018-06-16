//
// Created by Simon on 21/03/2018.
//

#include "horn_afp.h"

template <>
void HORN::AFPMinimization<FCA::LinClosure>(const theory &L, theory &Lc) {
    Lc.clear();

    if (L.empty())
        return;

    size_t attrNum = L.front().Premise().size();
    size_t implNum = L.size();

    int maximum_size = 0;

    // stack S
    std::list<FCA::BitSet> S;
    FCA::BitSet premise(attrNum), Lc_closure(attrNum), L_closure;
    FCA::BitSet C(attrNum), D(attrNum), M(attrNum);
    FCA::ImplicationInd copy;
    bool found;
    M.set();

    // counters for LinClosure
    std::vector<size_t> l_count;
    std::vector<std::vector<size_t>> l_list;

    for (auto &impL : L){
        S.clear();
        S.emplace_back(impL.Premise());

        do{
            premise = S.back();
            S.pop_back();
            FCA::Closure::Apply(premise, Lc, Lc_closure);
            FCA::LinClosure::Apply(Lc_closure, L, L_closure, l_count, l_list);

            // in case we found a negative counter-ex
            if(Lc_closure != L_closure){
                found = false;

                for (auto &impLc : Lc){
                    C = impLc.Premise();
                    C &= Lc_closure;  // C := A n X

                    // we found a smaller neg-ex
                    if (C != impLc.Premise()){
                        FCA::LinClosure::Apply(C, L, D, l_count, l_list);
                        if (C != D){
                            found = true;
                            copy = impLc;
                            impLc = FCA::ImplicationInd(C, D);   // replacement in K
                            Lc_closure |= D;
                            S.emplace_back(Lc_closure);  // X may still be a negative ex

                            if (copy.Conclusion() != D){
                                S.emplace_back(copy.Premise());
                            }

                            if(S.size() > maximum_size){
                                maximum_size = (int) S.size();
                            }
                            break; // exit for may be changed --> not secured.
                        }
                    }

                }

                // no smaller counter-ex than X
                // add X --> L(X) to Lc
                if (!found) {
                    M.reset();
                    FCA::LinClosure::Apply(Lc_closure, L, M, l_count, l_list);
                    Lc.emplace_back(FCA::ImplicationInd(Lc_closure, M));
                }

            }

        } while (!S.empty());

    }

    //std::cout << "stack maximum size: " << Lc.size() << std::endl;

}
