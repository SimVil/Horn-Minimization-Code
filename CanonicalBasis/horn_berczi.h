/// \file horn_berczi.h
/// \author Simon Vilmin
/// \date 2018.
///
/// Defines Berczi Algorithm.
#ifndef ALGORITHMS_HORN_BERCZI_H
#define ALGORITHMS_HORN_BERCZI_H

#include <vector>
#include "fca_implications.h"
#include "fca_linclosure.h"

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
    std::vector<FCA::ImplicationInd> BercziMinimization(const std::vector<FCA::ImplicationInd> &L);

}


#endif //ALGORITHMS_HORN_BERCZI_H
