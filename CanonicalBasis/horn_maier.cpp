//
// Created by Simon on 06/02/2018.
//

#include "fca_closure_operators.h"
#include "horn_maier.h"


/// redundancyElimination: removes redundant implications from a given implicational basis.
std::vector<FCA::ImplicationInd> HORN::redundancyElimination(const std::vector<FCA::ImplicationInd> &L) {
    if (L.empty())
        return L;

    size_t attrNum = L.front().Premise().size();
    size_t implNum = L.size();

    std::vector<FCA::ImplicationInd> Lirr = L;
    FCA::BitSet closure(attrNum);
    FCA::ImplicationInd tmp;

    // to keep the order of L, we pop back, and insert at
    // the beginning (circular).

    for(size_t i = 0; i < implNum; ++i)
    {
        tmp = Lirr.front();
        Lirr.erase(Lirr.begin());

        FCA::LinClosure::Apply(tmp.Premise(), Lirr, closure);
        if(!tmp.Conclusion().is_subset_of(closure))  // L - {A --> B} |/= A --> B
        {
            Lirr.push_back(tmp);
        }
    }

    return Lirr;
}



/// getEquivalenceMatrix: gets (square) matrix of implied premises.
void HORN::getEquivalenceMatrix(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::BitSet> &matrix)
{
    if (L.empty())
        return;

    size_t implNum = L.size();  // number of implications
    size_t attrNum = L.front().Premise().size();  // size of attribute set
    matrix.resize(implNum);

    // we will not be interested in closure: we use a stub argument.
    // /!\ this can be unsecured /!\.
    FCA::BitSet tmp(attrNum);
    tmp.reset();

    for(size_t i = 0; i < implNum; ++i)
    {
        matrix[i].resize(implNum);
        matrix[i].reset();

        FCA::LinClosure::Apply(L[i].Premise(), L, tmp, 0, &matrix[i]);
    }

}

/// getEquivalenceClasses: gets all equivalence classes of a basis.
std::vector<FCA::BitSet> HORN::getEquivalenceClasses(const std::vector<FCA::ImplicationInd> &L)
{
    unsigned int implNum = L.size();
    std::vector<FCA::BitSet> classes;

    std::vector<FCA::BitSet> matrix;
    HORN::getEquivalenceMatrix(L, matrix);

    // Lbit: implications with premise to treat.
    FCA::BitSet Lbit(implNum);
    Lbit.set();

    for(size_t i = 0; i < implNum; ++i)
    {
        if(Lbit.test(i))
        {
            FCA::BitSet tmp(implNum); //dirty.
            tmp.reset();

            for(size_t j = i; j < implNum; ++j)
            {
                if(matrix[i].test(j) && matrix[j].test(i))  // premise of j is equivalent to premise of i
                {
                    tmp.set(j);
                    Lbit.reset(j);  // if j in class of i, no need to look for class of j.
                }
            }
            classes.push_back(tmp);
        }
    }

    // Note: XOR between all the classes should rise 1 for every bit/implication.
    // (equivalence partitions implications).
    return classes;
}



void HORN::MaierMinimization(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::ImplicationInd> &Lc) {

    Lc.clear();

    if (L.empty())
        return;

    // I - redundancy elimination
    std::vector<FCA::ImplicationInd> minL = HORN::redundancyElimination(L);
    unsigned int implNum = minL.size();
    unsigned int attrNum = minL.front().Premise().size();

    // II - Direct determination removal

    // get equivalence classes
    std::vector<FCA::BitSet> E_L = HORN::getEquivalenceClasses(minL);
    unsigned int Enum = E_L.size();

    // removing direct determination
    std::vector<FCA::ImplicationInd> tmpL = minL;
    FCA::BitSet implied(implNum);
    FCA::BitSet tmp(attrNum);
    FCA::BitSet closure(attrNum);
    int ddet = -1;
    int shift = 0;
    bool found = false;
    size_t e_x = 0;

    // Note: to stick as much as possible to theoretical complexity, we inverted the loops. Indeed, going through
    // all implications allow to remove implications in order, and in O(1). On the other way we would have needed
    // to add an O(B(L)) loop to obtain the final basis. This is because of the vector structure we rely on, not
    // allowing indexed removal. Passing through another structure would still require some time to convert the
    // final basis back into the original vector structure.

    std::vector<int> leftmoved(implNum, 0);
    int lastmove = 0;


    for(size_t imp = 0; imp < implNum; ++imp)
    {
        e_x = 0;
        found = false;

        if (imp > 0){
            if (leftmoved[imp - 1] != -1){
                leftmoved[imp] = leftmoved[imp - 1];
            } else {
                lastmove++;
                leftmoved[imp] = lastmove;
            }

        }

        while(e_x < Enum && !found)
        {
            if(E_L[e_x].test(imp))
            {
                implied = E_L[e_x];
                implied.reset(imp); // avoid direct determination to itself
                tmp = tmpL[imp].Conclusion();
                tmpL[imp].Conclusion().reset(); // makes a contradiction
                FCA::LinClosure::Apply(tmpL[imp].Premise(), tmpL, closure, 0, &implied, &ddet);
                tmpL[imp].Conclusion() |= tmp;
                found = true;
            }


            if (ddet != -1) // we found direct determination
            {
                E_L[e_x].reset(imp);
                tmpL[ddet].Conclusion() |= tmpL[imp].Conclusion();
                if (ddet > imp){
                    minL[ddet - shift].Conclusion() |= tmpL[imp].Conclusion();
                } else {
                    minL[ddet - leftmoved[ddet]].Conclusion() |= tmpL[imp].Conclusion();
                }

                tmpL[imp].Conclusion().reset(); // "deleting" implication
                tmpL[imp].Premise().reset(); // we must wait to truly remove it to keep
                                             // the sizes of E_L and minL coherent in LinClosure.
                minL.erase(minL.begin() + imp - shift);
                shift++;
                leftmoved[imp] = -1;
                ddet = -1;

            }

            e_x++;
        }
    }

    Lc = minL;
}