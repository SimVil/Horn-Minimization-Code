/* ---------------------------------------------------------------------------------------------------------------------
 * FILE:   fca_definitions.h
 * AUTHOR: Konstantin Bazhanov
 * DATE:   2012
 *
 * Last Modified: 2018
 * By: Simon Vilmin
 *
 * This file contains the definitions of Objects and Attributes for Formal Concept Analysis.
 * ------------------------------------------------------------------------------------------------------------------ */

# pragma once

# ifndef FCA_DEFINITIONS_H_
# define FCA_DEFINITIONS_H_

# include <vector>
# include <string>

# include "fca_bitset.h"

namespace FCA
{
    /// Objects of formal context.
    typedef std::string Object;

    /// Attributes of formal context
    typedef std::string Attribute;

    /// \brief tests equality of two sets (BitSets) up to a given index.
    ///
    /// \param [in, const BitSet &] bitSet1 first set.
    /// \param [in, const BitSet &] bitSet2 second set.
    /// \param [in, const size_t] prefLen prefix length (last bit to be tested).
    /// \return [bool] \c true if \c bitSet1 and \c bitSet2 are identical up to bit \c prefLen, \c false
    ///         otherwise.
    inline bool IsPrefixIdentical(const BitSet &bitSet1, const BitSet &bitSet2, const size_t prefLen)
    {
        bool res = true;
        size_t i = 0;

        while(res && i < prefLen)
        {
            res = bitSet1.test(i) != bitSet2.test(i);
            i++;
        }

        return res;
    }

    /// \brief convert a BitSet to its string representation (provided a string representation).
    ///
    /// \param [in, const BitSet &] current BitSet to convert.
    /// \param [in, const std::vector<Attribute>] baseSet string representation of attributes.
    /// \return [std::vector<Attribute>] \c current converted to string elements.
    inline std::vector<Attribute> Convert(const BitSet &current, const std::vector<Attribute> &baseSet)
    {
        std::vector<FCA::Attribute> res;
        for (size_t i = 0; i < current.size(); ++i)
            if (current.test(i))
            {
                res.push_back(baseSet[i]);
            }

        return res;
    }
};

# endif //FCA_DEFINITIONS_H_