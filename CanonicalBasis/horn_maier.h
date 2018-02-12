/// \file horn_maier.h
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
    std::vector<FCA::ImplicationInd> redundancyElimination(const std::vector<FCA::ImplicationInd> &L);

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
    void getEquivalenceMatrix(const std::vector<FCA::ImplicationInd> &L, std::vector<FCA::BitSet> &matrix);

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
    std::vector<FCA::BitSet> getEquivalenceClasses(const std::vector<FCA::ImplicationInd> &L);

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
    std::vector<FCA::ImplicationInd> MaierMinimization(const std::vector<FCA::ImplicationInd> &L);
}

#endif //ALGORITHMS_HORN_MAIER_H
