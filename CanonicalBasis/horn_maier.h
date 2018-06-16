/// \file horn_maier.h
/// \author Simon Vilmin
/// \date 2018.
///
/// Defines Maier Algorithm.


# pragma once

#ifndef ALGORITHMS_HORN_MAIER_H
#define ALGORITHMS_HORN_MAIER_H

#include <vector>
#include "fca_datastructures.h"
#include "fca_closure_operators.h"

namespace HORN {

    /// \brief removes redundant implications from a given implicational basis.
    ///
    /// We are given a basis L. Recall that an implication A --> B is redundant in L if
    /// L - {A --> B} |= A --> B. This is the first step of Maier Algorithm.
    ///
    /// \param [in] L a set of implications.
    /// \return a non-redundant cover of L.
    template <typename ClosureOperator>
    void redundancyElimination(const theory &L, theory &Lirr);

    /// \brief gets (square) matrix of implied premises.
    ///
    /// We are given a basis L. The aim is to find for each implication i of L, which of implications j of
    /// L have premise implied by i. In other words, we are looking for implications j such that Premise(j)
    /// is included in Closure(Premise(i)). Because we apply this procedure to each implication, we produce
    /// a |L| x |L| matrix M, such that M[i, j] = 1 iff Premise(i) --> Premise(j).
    /// To do it, we apply LinClosure to each premise with a BitSet V of size |L|. In LinClosure (cf documentation)
    /// i-th bit of V will be set to 1 if Premise(i) --> Premise(j), that is, count[j] = 0.
    ///
    /// \param [in] L the basis to do computations with.
    /// \param [in, out] matrix the matrix of implied premises.
    template <typename ClosureOperator>
    void getEquivalenceMatrix(const theory &L, std::vector<FCA::BitSet> &matrix);

    template <>
    void getEquivalenceMatrix<FCA::LinClosure>(const theory &L, std::vector<FCA::BitSet> &matrix);


    /// \brief gets all equivalence classes of a basis.
    ///
    /// We are given a matrix M of implied premises (over some basis L, see getEquivalenceMatrix). Implications of
    /// L can be partitioned in equivalence classes. Two implications i, j will be in the same class if Premise(i)
    /// <--> Premise(j) under L.
    /// To perform computations, we do a run over M while keeping a set of implications we have to assign to some
    /// class. Roughly, the principle/pseudo-code goes as follows:
    /// \code
    ///     Compute the Matrix of implied premises (see getEquivalenceMatrix)
    ///     Initialize a list L of implications to assign (BitSet);
    ///     Initialise a set E of equivalence classes;
    ///
    ///     For each implication i:
    ///         if i is not assigned:
    ///             declare a new classe E_i in E;
    ///             for each implication j:
    ///                 if Premise(i) <--> Premise(j):
    ///                     add j to E_i;
    ///                     remove j from implications to assign;
    ///
    ///     return E;
    /// \endcode
    ///
    /// \param [in] L implication basis.
    /// \return a set of equivalence classes.
    template <typename ClosureOperator>
    std::vector<FCA::BitSet> getEquivalenceClasses(const theory &L);


    /// \brief implementation of Maier algorithm for implicational basis minimization.
    ///
    /// This function is the implementation of the algorithm proposed by Maier in "Minimum Covers in Relational
    /// Databases" (1980). The principle is the following:
    ///     - remove redundant implications (see redundancyElimination),
    ///     - remove direct determination (see getEquivalenceClasses).
    ///
    /// For the first point, we only need to apply LinClosure to each premise of \c L. Direct determination relies
    /// on \a equivalence \a classes. For a given premise A, we must find all other implications with premises
    /// equivalent to A. If we can reach such a premise B without the implications we found, we say that A directly
    /// determines B. Thus, we can remove the implication having A as a premise, and update the implication related
    /// to B. Note: in order to stick as much as possible to theoretical complexity, we exchanged the running process.
    /// That is, instead of going over all equivalence classes and then over all implications, we go over all
    /// implications and then over all equivalence classes. This is because removing an implication and shifting
    /// the vector in memory can be consuming. Thus, doing the first way would result in an O(B(L)) loop more.
    ///
    /// \param [in] L the basis to minimize.
    /// \return a minimized version of \c L.
    template <typename ClosureOp1, typename ClosureOp2>
    void MaierMinimization(const theory &L, theory &Lc);

}


/// redundancyElimination: removes redundant implications from a given implicational basis.
template <typename ClosureOperator>
void HORN::redundancyElimination(const theory &L, theory &Lirr) {

    Lirr.clear();
    if (L.empty())
        return;

    size_t attrNum = L.front().Premise().size();
    size_t implNum = L.size();

    Lirr = L;
    FCA::BitSet closure(attrNum);
    FCA::ImplicationInd tmp;

    // to keep the order of L, we pop back, and insert at
    // the beginning (circular).

    for(auto i = (int) implNum - 1; i >= 0; --i)
    {
        tmp = Lirr[i];
        Lirr[i] = Lirr.back();
        Lirr.pop_back();

        ClosureOperator::Apply(tmp.Premise(), Lirr, closure);
        if(!tmp.Conclusion().is_subset_of(closure))  // L - {A --> B} |/= A --> B
            Lirr.push_back(tmp);
    }

}


/// getEquivalenceMatrix: gets (square) matrix of implied premises.
template <typename ClosureOperator>
void HORN::getEquivalenceMatrix(const theory &L, std::vector<FCA::BitSet> &matrix)
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

        ClosureOperator::Apply(L[i].Premise(), L, tmp, 0, &matrix[i]);
    }

}


/// getEquivalenceClasses: gets all equivalence classes of a basis.
template <typename ClosureOperator>
std::vector<FCA::BitSet> HORN::getEquivalenceClasses(const theory &L)
{
    size_t implNum = L.size();
    std::vector<FCA::BitSet> classes;

    std::vector<FCA::BitSet> matrix;
    HORN::getEquivalenceMatrix<ClosureOperator>(L, matrix);

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


template <typename ClosureOp1, typename ClosureOp2>
void HORN::MaierMinimization(const theory &L, theory &Lc) {

    Lc.clear();

    if (L.empty())
        return;

    // I - redundancy elimination
    theory minL;
    HORN::redundancyElimination<ClosureOp1>(L, minL);
    size_t implNum = minL.size();
    size_t attrNum = minL.front().Premise().size();

    // II - Direct determination removal

    // get equivalence classes
    std::vector<FCA::BitSet> E_L = HORN::getEquivalenceClasses<ClosureOp2>(minL);
    size_t Enum = E_L.size();

    // removing direct determination
    std::vector<FCA::ImplicationInd> tmpL = minL;
    FCA::BitSet implied(implNum);
    FCA::BitSet tmp(attrNum);
    FCA::BitSet closure(attrNum);
    //int ddet = -1;
    size_t ddet = L.size() + 1; // out of bounds : equiv to -1
    bool found = false;
    size_t e_x = 0;

    // Note: to stick as much as possible to theoretical complexity, we inverted the loops. Indeed, going through
    // all implications allow to remove implications in order, and in O(1). On the other way we would have needed
    // to add an O(B(L)) loop to obtain the final basis. This is because of the vector structure we rely on, not
    // allowing indexed removal. Passing through another structure would still require some time to convert the
    // final basis back into the original vector structure.

    std::vector<int> leftmoved(implNum, 0);
    int shift = 0;

    for(size_t imp = 0; imp < implNum; ++imp)
    {
        e_x = 0;
        found = false;

        // this imp has been moved already <shift> times (thus <shift> left steps).
        leftmoved[imp] = shift;

        // search for imp in equivalence classes.
        // Once we found the eq-class e_x of imp, we check for direct determination
        // found is for "we found the eq_class of imp".
        while(e_x < Enum && !found)
        {
            if(E_L[e_x].test(imp))
            {
                implied = E_L[e_x];
                implied.reset(imp); // avoid direct determination to itself
                tmp = tmpL[imp].Conclusion();
                tmpL[imp].Conclusion().reset(); // makes not informative implication
                ClosureOp1::Apply(tmpL[imp].Premise(), tmpL, closure, (size_t *) nullptr, &implied, &ddet, true);
                tmpL[imp].Conclusion() |= tmp;
                found = true;
            }


            if (ddet < minL.size()) // we found direct determination
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
                ddet = L.size() + 1;

            }

            e_x++;
        }
    }

    Lc = minL;
}


#endif //ALGORITHMS_HORN_MAIER_H
