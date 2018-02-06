/// \file horn_maier.h
///
/// Defines Maier Algorithm.


# pragma once

#ifndef ALGORITHMS_HORN_MAIER_H
#define ALGORITHMS_HORN_MAIER_H

#include <vector>
#include "fca_datastructures.h"

namespace HORN {
    /// \brief implementation of Maier algorithm for implicational basis minimization.
    ///
    /// This function is the implementation of the algorithm proposed by Maier in "Minimum Covers in Relational
    /// Databases" (1980). The principle is the following:
    ///     - remove redundant implications,
    ///     - remove direct determination.
    ///
    /// For the first point, we only need to apply LinClosure to each premise of \c L. Direct determination relies
    /// on \a equivalence \a classes. For a given premise A, we must find all other implications with premises
    /// equivalent to A. If we can reach such a premise B without the implications we found, we say that A directly
    /// determines B. Thus, we can remove the implication having A as a premise, and update the implication related
    /// to B.
    ///
    /// \param [in, const &] L the basis to minimize.
    /// \return [vector<ImplicationInd>] a minimized version of \c L.
    std::vector<FCA::ImplicationInd> MaierMinimization(const std::vector<FCA::ImplicationInd> &L);

    std::vector<FCA::BitSet> GetEquivalenceMatrix(const std::vector<FCA::ImplicationInd> &L);
}


#endif //ALGORITHMS_HORN_MAIER_H
