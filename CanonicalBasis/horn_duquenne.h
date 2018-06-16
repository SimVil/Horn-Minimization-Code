/// \file horn_duquennei.h
/// \author Simon Vilmin
/// \date 2018.
///
/// Defines Duquenne Algorithm.
#ifndef ALGORITHMS_HORN_DUQUENNE_H
#define ALGORITHMS_HORN_DUQUENNE_H

#include "fca_closure_operators.h"
#include "fca_implications.h"

typedef std::vector<FCA::ImplicationInd> theory;

namespace HORN {

    /// \brief Lectic ordering.
    ///
    /// This function implements lectic comparison between two implications. The comparison is made on premises
    /// of implications. For the recall, assume that the attribute set is linearly ordered. A < B if the smallest
    /// element in which they differ belongs to B.
    ///
    /// \param I_1 implication
    /// \param I_2 implication
    /// \return true if I_1 < I_2, false otherwise.
    bool LecticComp(const FCA::ImplicationInd &I_1, const FCA::ImplicationInd &I_2);

    /// \brief Implementation of Duquenne variation of MinCover.
    ///
    /// We implement the algorithm provided by Duquenne in "Some variations on Alan Day’s algorithm for calculating
    /// canonical basis of implications" (2007). First we perform quasi-closure and redundancy elimination. After lectic
    /// ordering, we use recursive property of pseudo-closed sets to build the canonical base. The first step is
    /// performed by the function getQuasiClosed. The principle is:
    /// \code
    ///     for each A --> B in L
    ///         A := L-(A)      // L- := L - A --> B
    ///         if B not in A then
    ///             this is no redundant implication
    ///             A --> B := A --> A u B
    ///
    ///     LecticOrdering(L)
    ///     Lc := {}
    ///
    ///     for each A --> B in L
    ///         for each C --> D in Lc
    ///             if A in C and D not in A then
    ///                 go to next A --> B in L
    ///         B := L(A)
    ///         add A --> B to Lc
    ///
    ///     return Lc
    /// \endcode
    ///
    /// \tparam ClosureOperator an operator for computing closure (input, system, result).
    /// \param L a system to minimize
    /// \param Lc the canonical representation of L.
    template<typename ClosureOperator>
    void DuquenneMinimization(const theory &L, theory &Lc);

    /// \brief specified implementation of Duquenne minimization, based on improvements of LinClosure.
    ///
    /// \param L system to minimize.
    /// \param Lc canonical representation of L.
    template<>
    void DuquenneMinimization<FCA::LinClosure>(const theory &L, theory &Lc);

    void getQuasiClosed(const theory &L, theory &Lc);

}


// instanciation of ClosureOperator template for Duquenne
template <typename ClosureOperator>
void HORN::DuquenneMinimization(const theory &L, theory &Lc) {

    getQuasiClosed(L, Lc); // Lc contains quasi-closure on the left, redundant imp are discarded
    std::sort(Lc.begin(), Lc.end(), LecticComp); // lectic ordering on premises.

    theory Lm;
    FCA::ImplicationInd tmp;
    size_t size = Lc.size() - 1;
    size_t j = 0;
    auto i = (int) size;
    bool redundant;

    // run over Lc
    while(i >= 0){
        j = 0;
        redundant = false;
        tmp = Lc[i];

        // bool redundant avoids using break.
        // run over Lm (implications P --> Q)
        while(j < Lm.size() && !redundant){

            // check pseudo-closedness for P: for some A --> L(A) (A pseudo-closed),
            // if A in P and L(A) not in P, P is not pseudo-closed.
            if (Lm[j].Premise().is_subset_of(tmp.Premise()) &&
                !Lm[j].Conclusion().is_subset_of(tmp.Premise())){

                // to limit computations, get rid of redundant P --> Q.
                Lc[i] = Lc.back();
                Lc.pop_back();
                redundant = true;
            }
            ++j;
        }

        // if the current implication A --> B is not redundant, A is pseudo-closed,
        // we add A --> L(A) to Lm.
        if(!redundant){
            ClosureOperator::Apply(tmp.Conclusion(), Lc, tmp.Conclusion());
            Lm.push_back(tmp);
        }

        --i;

    }

    Lc = Lm;

}

#endif //ALGORITHMS_HORN_DUQUENNE_H
