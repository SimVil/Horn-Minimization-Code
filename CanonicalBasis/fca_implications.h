/* ---------------------------------------------------------------------------------------------------------------------
 * FILE:   fca_implications.h
 * AUTHOR: Konstantin Bazhanov
 * DATE:   2012
 *
 * Last Modified: 2018
 * By: Simon Vilmin
 *
 * This file contains the definitions of implications for Formal Concept Analysis and Horn Minimization.
 * ------------------------------------------------------------------------------------------------------------------ */

# pragma once

# ifndef FCA_IMPLICATIONS_H_
# define FCA_IMPLICATIONS_H_

# include "fca_definitions.h"

namespace FCA
{
    /// \brief string representation of implications.
    ///
    /// implications are made among attributes. For the recall, they can be formalized as
    ///  A --> B where A, B are two subsets of the attribute universe. A is called the premise,
    /// and B the conclusion of the implication.
    class Implication
    {
    protected:
        /// implication premise
        std::vector<Attribute> m_premise;

        /// implication conclusion
        std::vector<Attribute> m_conclusion;
    public:

        /// \brief Constructor.
        Implication() {}

        /// \brief Constructor. Builds an implication with given premise and conclusion.
        ///
        /// \param [in, const &] iPremise  attribute set for premise.
        /// \param [in, const &] iConclusion attriute set for conclusion.
        Implication(const std::vector<Attribute> &iPremise, const std::vector<Attribute> &iConclusion) : m_premise(iPremise), m_conclusion(iConclusion) {}

        /// \brief Getter. Get for premise.
        ///
        /// \return [vector<Attribute> &] premise of the implication.
        std::vector<Attribute> &Premise() { return m_premise; }

        /// \brief Const Getter. For premise.
        ///
        /// \return [const vector<Attribute> &] premise of the implication.
        const std::vector<Attribute> &Premise() const {return m_premise; }

        /// \brief Getter. Get for conclusion.
        ///
        /// \return [vector<Object> &] conclusion of the implication.
        std::vector<Object> &Conclusion() { return m_conclusion; }

        /// \brief const Getter. For conclusion.
        ///
        /// \return [const vector<Object> &] conclusion of the implication.
        const std::vector<Object> &Conclusion() const { return m_conclusion; }
    };

    /// \brief BitSet representation of Implications
    ///
    /// This is the echo of class Implication with bitmaps. This is the class/structure we will use for
    /// manipulating implications, computing closure and so forth. Here, struct acts as a class (because all
    /// members have explicit properties).
    struct ImplicationInd
    {
    protected:
        /// implication premise
        BitSet m_premise;

        /// implication conclusion
        BitSet m_conclusion;
    public:

        /// \brief Default constructor.
        ImplicationInd() {}

        /// \brief Constructor. Builds an implication with given premise and conclusion.
        ///
        /// \param [in, const &] iPremise  attribute set for premise.
        /// \param [in, const &] iConclusion attriute set for conclusion.
        ImplicationInd(const BitSet &iPremise, const BitSet &iConclusion) : m_premise(iPremise), m_conclusion(iConclusion) {}

        /// \brief Getter. Get for premise.
        ///
        /// \return [Bitset &] premise of the implication.
        BitSet &Premise() { return m_premise; }

        /// \brief Const Getter. For premise.
        ///
        /// \return [const vector<Attribute> &] premise of the implication.
        const BitSet &Premise() const { return m_premise; }

        /// \brief Getter. Get for conclusion.
        ///
        /// \return [vector<Object> &] conclusion of the implication.
        BitSet &Conclusion() { return m_conclusion; }

        /// \brief const Getter. For conclusion.
        ///
        /// \return [const vector<Object> &] conclusion of the implication.
        const BitSet &Conclusion() const { return m_conclusion; }
    };

    /// \brief converts an implication from BitSets to strings.
    ///
    /// \param [in, const &] impl the bitmap representation of an implication.
    /// \param [in, const &] baseSet string representation of the universe.
    /// \return [Implication] \c impl converted to string.
    Implication Convert(const ImplicationInd &impl, const std::vector<Attribute> &baseSet);

    /// \brief converts a list of implications to strings.
    ///
    /// \param [in, const &] impl implication (bitsets) list.
    /// \param [in, const &] baseSet string representation of the universe.
    /// \return [vector<Implication>] \c impl converted to strings.
    std::vector<Implication> Convert(const std::vector<ImplicationInd> &impl, const std::vector<Attribute> &baseSet);

    /// \brief converts a system of string implications to bitset representation.
    ///
    /// \param [in, const &] current attributes present in implications.
    /// \param [in, const &] attributes attribute universe.
    /// \param [in, const &] implications string implications.
    /// \param [in/out, &] cCurrent attributes present in implications (bitset).
    /// \param [in/out, &] cImplications bitset representations of given implications.
    void Convert(const std::vector<Attribute> &current, const std::vector<Attribute> &attributes, const std::vector<Implication> &implications, 
                 BitSet &cCurrent, std::vector<ImplicationInd> &cImplications);	
};

# endif //FCA_IMPLICATIONS_H_