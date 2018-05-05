# pragma once

# ifndef FCA_MINIMIZATION_H_
# define FCA_MINIMIZATION_H_

# include <vector>

# include "fca_datastructures.h"
# include "fca_closure_operators.h"

namespace FCA
{
    // template for general closure operator use
    template <typename ClosureOperator>
    void MinimalCover(const theory &L, theory &Lc);

    // specialization for fine-tuned use of LinClosure
    template<>
    void MinimalCover<FCA::LinClosure>(const theory &L, theory &Lc);

    std::vector<ImplicationInd> MinimizeBasis(const std::vector<ImplicationInd> &implSet);
};



// ==== template instanciation for general closure operators ====

// template definition of MinCover
template <typename ClosureOperator>
void FCA::MinimalCover(const theory &L, theory &Lc) {

    Lc.clear();
    if (L.empty()){ return; }

    // Two points:
    //  (i)  A --> B becomes A --> A u B (ease later computations)
    //  (ii) fulfilling Lc
    for (auto imp : L){
        imp.Conclusion() |= imp.Premise();
        Lc.emplace_back(imp);
    }

    // right-closing: A --> B becomes A --> Lc(A)
    for (auto &imp : Lc){
        ClosureOperator::Apply(imp.Conclusion(), Lc, imp.Conclusion());
    }

    size_t size = Lc.size() - 1;
    FCA::ImplicationInd imp;

    // redundancy elimination: use of swapping to ease deletion of implications
    for (int i = (int) size; i >= 0; --i){
        imp = Lc[i];
        Lc[i] = Lc.back();
        Lc.pop_back();

        ClosureOperator::Apply(imp.Premise(), Lc, imp.Premise());
        if (imp.Premise().is_proper_subset_of(imp.Conclusion())){
            Lc.push_back(imp);
        }
    }
}



# endif //FCA_MINIMIZATION_H_