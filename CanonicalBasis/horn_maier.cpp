/// \file horn_maier.cpp
/// \author Simon Vilmin
/// \date 2018.
///
/// Implements functions of horn_maier.h

#include "fca_closure_operators.h"
#include "horn_maier.h"


/// getEquivalenceMatrix: gets (square) matrix of implied premises.
template <>
void HORN::getEquivalenceMatrix<FCA::LinClosure>(const theory &L, std::vector<FCA::BitSet> &matrix)
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

    std::vector<size_t> count;
    std::vector<std::vector<size_t>> list;

    FCA::LinClosure::initCounters(L, count, list);

    for(size_t i = 0; i < implNum; ++i)
    {
        matrix[i].resize(implNum);
        matrix[i].reset();

        FCA::LinClosure::Apply(L[i].Premise(), L, tmp, count, list, nullptr, &matrix[i]);
    }

}