/// \file Test_Maier_Unit.cpp
/// \author Simon Vilmin.
/// \date 2018
///
/// \brief Contains a basic set of unit test cases to check correctness of Maier Algorithm for minimization. Made with
/// catch2.
///
/// The code is not expected to be used in any commercial software. Its aim is to test research algorithms. As
/// a consequence, we are not going to test validity of input/output formatting. We want to test correctness of
/// algorithm, thus we will focus on special cases of basis more than all the cases of wrong manipulation of the
/// code leading to a crash.
/// Tests are:
///     - empty basis
///     - ...
/// Tests will split over various parts of the algorithm (see horn_maier.h for more details).


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch.h"
#include "../CanonicalBasis/test_functions.h"
#include "../CanonicalBasis/horn_maier.h"
#include "Reader.h"


void SequenceTesting(const std::string &path, const std::string &extension, const std::vector<std::string> &filenames,
                     const unsigned &tests_nb );

/// \brief Test of empty basis.
///
/// We give as an input to the test an empty basis L. Quite easily, the algorithm should stop almost immediately:
///     - Input: L = {}
///     - Expected output:
///         -# irredundancy: Lc = {}
///         -# maier minimization:  Lc = {}
TEST_CASE("Maier Algorithm: empty basis")
{
    std::vector<FCA::ImplicationInd> L;
    std::vector<FCA::ImplicationInd> minL = HORN::redundancyElimination(L);
    std::vector<FCA::BitSet> matrix;

    CHECK(minL.empty());

    HORN::getEquivalenceMatrix(L, matrix);

    CHECK(matrix.empty());

    minL = HORN::MaierMinimization(L);

    REQUIRE(minL.empty());
}

/// \brief Test of various contradiction possibilities.
///
/// The input will be implicational basis with contradictions. Contradictions are implications A --> {}. The algorithm
/// should remove them because they are redundant:
///     - Input: L with implication(s) A --> {}
///     - Expceted output:
///         -# irredundancy: L_c with contradiction removed
///         -# maier minimization: a minimal basis.
/// \note For this test, we use automated file testing. For each basis we have 3 files (input, inter, output) containing
/// respectively the original input, the basis after redundancy elimination, the expected minimum basis. Using a reader
/// we can check for equivalence after each steps.
TEST_CASE("Maier Algorithm: contradictions")
{
    std::string path = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/Contradictions/";
    std::string extension = ".txt";

    unsigned int tests_nb = 2;  // number of contradictions cases to run.
    unsigned int seq_len = 3;  // input, inter, output
    std::vector<std::string> filenames = {"input", "inter", "output"};

    SequenceTesting(path, extension, filenames, tests_nb);

}



TEST_CASE("Maier Algorithm: non-closed empty set")
{
    std::string path = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/NonClosedEmptySet/";
    std::string extension = ".txt";

    unsigned int tests_nb = 1;
    unsigned int seq_len = 3;
    std::vector<std::string> filenames = {"input", "inter", "output"};

    SequenceTesting(path, extension, filenames, tests_nb);
}



void SequenceTesting(const std::string &path, const std::string &extension, const std::vector<std::string> &filenames,
                     const unsigned &tests_nb )
{
    unsigned int seq_len = filenames.size();
    std::vector<std::string> buffer(seq_len);
    std::vector<std::vector<FCA::Implication>> basis(seq_len);
    std::vector<FCA::Attribute> sigma_s;
    std::vector<FCA::ImplicationInd> L;
    std::vector<FCA::ImplicationInd> minL;
    std::vector<FCA::Implication> buffL;
    FCA::BitSet sigma;

    for(int i = 1; i <= tests_nb; ++i)
    {
        for(int j = 0; j < seq_len; ++j)
        {
            buffer[j] = path;
            buffer[j] += filenames[j];
            buffer[j] += "_";
            buffer[j] += std::to_string(i);
            buffer[j] += extension;
        }

        Reader::FileSequenceHandler(buffer, basis, sigma_s);
        FCA::Convert(sigma_s, sigma_s, basis[0], sigma, L);

        minL = HORN::redundancyElimination(L);
        buffL = FCA::Convert(minL, sigma_s);

        REQUIRE(IsVectorOfImplicationsIdentical(buffL, basis[1]));

        minL = HORN::MaierMinimization(L);
        buffL = FCA::Convert(minL, sigma_s);

        REQUIRE(IsVectorOfImplicationsIdentical(buffL, basis[2]));

        for(int j = 0; j < seq_len; ++j)
        {
            basis[j].clear();
        }

        L.clear();

    }
}