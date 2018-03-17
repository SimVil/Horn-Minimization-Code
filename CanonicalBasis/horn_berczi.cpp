//
// Created by Simon on 09/02/2018.
//

#include "horn_berczi.h"


void HORN::BercziMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc) {
    Lc.clear();

    if (L.empty())
        return;


    unsigned long implNum = L.size();
    unsigned long attrNum = L.front().Premise().size();
    size_t min_index = 0;
    bool found = false; // allows to deal with empty canonical basis.

    FCA::BitSet closure_L(attrNum);
    FCA::BitSet closure_minL(attrNum);
    FCA::BitSet min_L(attrNum);
    FCA::BitSet min_minL(attrNum);
    min_minL.set();

    FCA::BitSet closed(implNum);
    closed.reset();

    while (!closed.all())
    {
        for (size_t imp = 0; imp < implNum; imp++)
        {
            if(!closed.test(imp))
            {
                FCA::LinClosure::Apply(L[imp].Premise(), L, closure_L);
                FCA::LinClosure::Apply(L[imp].Premise(), Lc, closure_minL);

                if(closure_L != closure_minL && (closure_minL.is_subset_of(min_minL) || closure_minL.count() < min_minL.count()))
                {
                    min_minL = closure_minL;
                    min_L = closure_L;
                    min_index = imp;
                    found = true;
                }

                if (closure_L == closure_minL)
                {
                    closed.set(imp);
                }
            }
        }

        if(found)
        {
            Lc.emplace_back(FCA::ImplicationInd(min_minL, min_L));
            closed.set(min_index);
            min_index = 0;
            min_minL.set();
            found = false;
        }

    }
}

void HORN::BodyBercziMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc) {
    Lc.clear();

    if (L.empty())
        return;


    unsigned long implNum = L.size();
    unsigned long attrNum = L.front().Premise().size();
    size_t min_index = 0;
    bool found = false; // allows to deal with empty canonical basis.

    FCA::BitSet closure_L(attrNum);
    FCA::BitSet closure_minL(attrNum);
    FCA::BitSet min_L(attrNum);
    FCA::BitSet min_minL(attrNum);
    min_minL.set();

    FCA::BitSet closed(implNum);
    closed.reset();

    while (!closed.all())
    {
        for (size_t imp = 0; imp < implNum; imp++)
        {
            if(!closed.test(imp))
            {
                FCA::LinClosure::Apply(L[imp].Premise(), L, closure_L);
                FCA::LinClosure::Apply(L[imp].Premise(), Lc, closure_minL);

                if(closure_L != closure_minL && (closure_minL.is_subset_of(min_minL) || closure_minL.count() < min_minL.count()))
                {
                    min_minL = closure_minL;
                    min_L = closure_L;
                    min_index = imp;
                    found = true;
                }

                if (closure_L == closure_minL)
                {
                    closed.set(imp);
                }
            }
        }

        if(found)
        {
            Lc.emplace_back(FCA::ImplicationInd(L[min_index].Premise(), min_L));
            closed.set(min_index);
            min_index = 0;
            min_minL.set();
            found = false;
        }

    }
}