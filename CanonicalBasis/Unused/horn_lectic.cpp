//
// Created by Simon on 16/03/2018.
//

#include "horn_lectic.h"
#include "test_functions.h"


bool HORN::SizeComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2) {
//    size_t size = I_1.Premise().size();
//    size_t i = 0;
//    bool eq; // = I_1.Premise().count() < I_2.Premise().count();
//
//    do {
//        eq = I_1.Premise().test(i) == I_2.Premise().test(i);
//        i++;
//    } while (i < size && eq);
//
//    eq = (eq || !I_1.Premise().test(i - 1));
//
//    return eq;
    return I_1.Premise().count() < I_2.Premise().count();
}

bool HORN::InclusionComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2) {
    return I_1.Premise().is_subset_of(I_2.Premise());
}

void HORN::LecticMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc) {
    Lc.clear();

    if (L.empty())
        return;

    std::vector<FCA::ImplicationInd> Lcopy = L;
    FCA::BitSet Lclosure, LCclosure;

    std::sort(Lcopy.begin(), Lcopy.end(), SizeComp);

    // PrintImplications(std::cout, FCA::Convert(Lcopy, {"a", "b", "c",  "d"}));

    for(auto &imp : Lcopy){
        FCA::LinClosure::Apply(imp.Premise(), L, Lclosure);
        FCA::LinClosure::Apply(imp.Premise(), Lc, LCclosure);

        if (Lclosure != LCclosure){
            //Lc.emplace_back(FCA::ImplicationInd(LCclosure, Lclosure));
            Lc.emplace_back(FCA::ImplicationInd(imp.Premise(), Lclosure));
        }


    }

}
