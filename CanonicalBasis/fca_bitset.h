/* ---------------------------------------------------------------------------------------------------------------------
 * FILE:   fca_bitset.h
 * AUTHOR: Konstantin Bazhanov
 * DATE:   2012
 *
 * Last Modified: 2018
 * By: Simon Vilmin
 *
 * This file defines the class BitSet, representing sets through bitmaps. The implementation relies on the library
 * included in fca_config.h.
 * ------------------------------------------------------------------------------------------------------------------ */

# pragma once

# ifndef FCA_BITSET_H_
# define FCA_BITSET_H_

# include "fca_config.h"

# ifdef BOOST_BITSET_ON
# include <boost/dynamic_bitset.hpp>
# endif //BOOST_BITSET_ON

# ifdef MAGIC_BITSET_ON
# include <src/bm.h>
# endif //MAGIC_BITSET_ON

namespace FCA
{
    /// \class BitSet
    ///
    /// \brief describes sets as characteristic vectors (bitmaps).
    class BitSet
    {
    public:

        /// \brief Constructor. Initialize a set of size \c new_size. The set is initialized to 0.
        ///
        /// \param [in, const] new_size size of the set. Default value: 0
        BitSet(const size_t new_size = 0);

        /// \brief Constructor by copy. Initialize a new BitSet as a copy of \c a.
        ///
        /// \param [in, const &] a a BitSet to copy.
        BitSet(const BitSet &a);

        /// \brief Tests whether the set is empty or not.
        ///
        /// \return [bool] \c true if the set is empty (all bits to 0), \c false otherwise.
        bool none() const;

        /// \brief Tests whether \c ind -th bit of the set is 1.

        /// \param [in, const] ind
        /// \return [bool] \c true if set[ind] == 1, \c false otherwise.
        bool test(const size_t ind) const;

        /// \brief Set the \c ind -th bit to 1.

        /// \param [in, const] ind index of the bit to set.
        void set(const size_t ind);

        /// \brief Set all the bits to 1.
        void set();

        /// \brief Set the \c ind -th bit to 0.

        /// \param [in, const] ind index of bit to reset.
        void reset(const size_t ind);

        /// \brief Set all the bits to 0.
        void reset();

        /// \brief Flip all bits to their opposite value.
        ///
        /// Note: this can be used as a "complement" function with respect to the universe we are working with.
        void flip();

        /// \brief Checks whether \c a is a subset of our set (large inclusion).
        ///
        /// \param [in, const &] a the set to test inclusion with.
        /// \return [bool] \c true if inclusion is verified, \c false otherwise.
        bool is_subset_of(const BitSet &a) const;

        /// \brief Tests whether \c a is properly contained in our set.
        ///
        /// \param [in, const &] a the set to be tested.
        /// \return [bool] \c true if inclusion is verified, \c false otherwise.
        bool is_proper_subset_of(const BitSet &a) const;

        /// \brief Gets the size of the bitmap.
        ///
        /// \return [size_t] effective size of the bitmap.
        size_t size() const;

        /// \brief Resize the bitmap.
        ///
        /// \param [in, const] new_size the new size of the set.
        void resize(const size_t new_size);

        /// \brief counts the number of elements in the set (numbers of bits to 1).
        ///
        /// \return [size_t] the number of elements set to 1.
        size_t count() const;

        /// \brief Override. Union operation.
        ///
        /// \param [in, const &] a set to perform union with.
        /// \return [BitSet &] the operation is made on \c this.
        BitSet &operator |=(const BitSet &a);

        /// \brief Override. Difference operation.
        ///
        /// \param [in, const &] a set to perform difference with.
        /// \return [BitSet &] the operation is made on \c this.
        BitSet &operator -=(const BitSet &a);

        /// \brief Override. Intersection operation.
        ///
        /// \param [in, const &] a set to intersect with.
        /// \return [BitSet &] the operation is made on \c this.
        BitSet &operator &=(const BitSet &a);

        /// \brief Override. Tests equality between sets.
        ///
        /// \param [in, const &] a set to compare.
        /// \return [bool] \c true if the two sets are equal, \c false otherwise.
        bool operator ==(const BitSet &a) const;

        /// \brief Override. Tests difference between sets.
        ///
        /// \param [in, const &] a set to compare.
        /// \return [bool] \c true if the two sets are different, \c false otherwise.
        bool operator !=(const BitSet &a) const;

        /// \brief Override. Lexicographical comparison.
        /// \param [in, const &] a other set.
        /// \return [bool] \c true if a is lexicographically greater than our set,\ false otherwise.
        bool operator <(const BitSet &a) const;

    private:
# ifdef BOOST_BITSET_ON
        /// Bitmap representing a set. The template specification defines the kind of memory block we rely on.
        /// (unsigned long is 8 bytes).
        boost::dynamic_bitset<unsigned long> m_bs;
# endif //BOOST_BITSET_ON

# ifdef MAGIC_BITSET_ON
        bm::bvector<> m_bs;
# endif //MAGIC_BITSET_ON
    };

    /// \brief Override. Difference operation.

    /// \param [in, const &] a set.
    /// \param [in, const &] b set.
    /// \return [BitSet] result of \c a - \c b.
    BitSet operator -(const BitSet &a, const BitSet &b);

    /// \brief Override. Intersection operation.

    /// \param [in, const &] a set.
    /// \param [in, const &] b set.
    /// \return [BitSet] result of \c a & \c b.
    BitSet operator &(const BitSet &a, const BitSet &b);
};

# endif //FCA_BITSET_H_