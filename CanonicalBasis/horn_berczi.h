/// \file horn_berczi.h
/// \author Simon Vilmin
/// \date 2018.
///
/// Defines Berczi Algorithm.
#ifndef ALGORITHMS_HORN_BERCZI_H
#define ALGORITHMS_HORN_BERCZI_H

#include <vector>
#include "fca_implications.h"
#include "fca_closure_operators.h"

typedef std::vector<FCA::ImplicationInd> theory;

namespace HORN
{

    /// \brief Implementation of Berczi Algorithm for implicational basis minimization.
    ///
    /// Ths function is the implementation of the algorithm proposed by Berczi and al. in "Directed Hypergraphs and
    /// Horn minimization". It relies on a bounding theorem of Boros and al. Initially, this algorithm was written
    /// in hypergraphs framework, but it can also be made out of the implicational one as presented here. The principle
    /// is the following:
    ///  \code
    ///     Initialize a new basis Lmin to {};
    ///     Initialize the bitset U of used bodies of L in Lmin; // all of them at the beginning
    ///
    ///     While U has bits to 0 do
    ///         for each body B of L such that U(B) == 0
    ///             if (Lmin(B) != L(B) and Lmin(B) is minimum) then
    ///                 keep Lmin(B), L(B), U(B);
    ///
    ///             if (Lmin(B) == L(B)) then
    ///                 set U(B) to 1;   // B is useless to build because already closed.
    ///
    ///         add Lmin(B) --> L(B) to Lmin;
    ///         set U(B) to 1;
    ///
    ///     return Lmin;
    /// \endcode
    ///
    /// \param L [in] the basis to minimize.
    /// \return the Duquenne-Guigues basis of L.
    template <typename ClosureOperator>
    void BercziMinimization(const theory &L, theory &Lc);

    template <>
    void BercziMinimization<FCA::LinClosure>(const theory &L, theory &Lc);
}


// ==== template instanciation for general closure operators ====

// template def of BercziMinimization
template <typename ClosureOperator>
void HORN::BercziMinimization(const theory &L, theory &Lc){

    Lc.clear();
    if(L.empty()){ return; }

    std::vector<FCA::BitSet> L_closures;
    std::vector<FCA::BitSet> Lc_closures;
    FCA::BitSet tmp;

    // compute and stack the closures of premises of L
    for(auto &imp : L){
        ClosureOperator::Apply(imp.Premise(), L, tmp);
        L_closures.emplace_back(tmp);
        Lc_closures.emplace_back(imp.Premise());
    }

    size_t implNum = L.size();
    size_t attrNum = L.front().Premise().size();
    size_t min_index = 0;
    bool found = false; // allows to deal with empty canonical basis.

    FCA::BitSet closure_L(attrNum);
    FCA::BitSet closure_minL(attrNum);
    FCA::BitSet min_L(attrNum);
    FCA::BitSet min_minL(attrNum);
    min_minL.set();

    FCA::BitSet closed(implNum);
    closed.reset();

    while(!closed.all()){
        for(size_t imp = 0; imp < implNum; imp++){
            if(!closed.test(imp)){
                ClosureOperator::Apply(Lc_closures[imp], Lc, Lc_closures[imp]);

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


#endif //ALGORITHMS_HORN_BERCZI_H
