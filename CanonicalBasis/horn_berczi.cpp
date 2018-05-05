//
// Created by Simon on 09/02/2018.
//

#include "horn_berczi.h"



template <>
void HORN::BercziMinimization<FCA::LinClosure>(const theory &L, theory &Lc){

    Lc.clear();
    if(L.empty()){ return; }

    std::vector<FCA::BitSet> L_closures;
    std::vector<FCA::BitSet> Lc_closures;
    FCA::BitSet tmp;

    // we only need one pair of structure. Because we will compute
    // closures in L only in one loop.
    std::vector<size_t> count;
    std::vector<std::vector<size_t>> list;

    // compute and stack the closures of premises of L
    for(auto &imp : L){
        FCA::LinClosure::Apply(imp.Premise(), L, tmp, count, list);
        L_closures.emplace_back(tmp);
        Lc_closures.emplace_back(imp.Premise());
    }

    size_t implNum = L.size();
    size_t attrNum = L.front().Premise().size();
    size_t min_index = 0;
    bool found = false; // allows to deal with empty canonical basis.
    count.clear();
    list.clear();

    FCA::BitSet closure_L(attrNum);
    FCA::BitSet closure_minL(attrNum);
    FCA::BitSet min_L(attrNum);
    FCA::BitSet min_minL(attrNum);
    min_minL.set();

    FCA::BitSet closed(implNum);
    closed.reset();

    std::vector<size_t> Lc_count;
    std::vector<std::vector<size_t>> Lc_list;

    while(!closed.all()){
        for(size_t imp = 0; imp < implNum; imp++){
            if(!closed.test(imp)){
                FCA::LinClosure::Apply(Lc_closures[imp], Lc, Lc_closures[imp], Lc_count , Lc_list, &attrNum);

                if (Lc_closures[imp] != L_closures[imp] &&
                    (Lc_closures[imp].is_subset_of(min_minL) || Lc_closures[imp].count() < min_minL.count())){

                    min_minL = Lc_closures[imp];
                    min_L = L_closures[imp];
                    min_index = imp;
                    found = true;
                }

                if (Lc_closures[imp] == L_closures[imp]){
                    closed.set(imp);
                }
            }
        }

        if(found){
            Lc.emplace_back(FCA::ImplicationInd(min_minL, min_L));
            closed.set(min_index);
            min_index = 0;
            min_minL.set();
            found = false;

            // update count and list for linclosure operations
            FCA::LinClosure::addImplication(Lc.back(), Lc_count, Lc_list);
        }
    }
}


