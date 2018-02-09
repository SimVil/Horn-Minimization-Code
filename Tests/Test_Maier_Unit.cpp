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
#include "Testers/MaierTester.h"



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
///     - Expected output:
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
    std::vector<std::string> filenames = {"input", "inter", "output"};

    MaierTester tester;

    std::cout << "Test Case: Contradictions" << std::endl << std::endl;
    tester.TestingSequence(path, extension, filenames, tests_nb);

}


/// \brief Test of basis with non-closed empty-set.
///
/// The input is a basis with some {} --> A implications. This should increase redundancy and direct
/// determination probability:
///     - Input: L with implications {} --> A
///  We just expect irredundant and minimal basis as results.
TEST_CASE("Maier Algorithm: non-closed empty set")
{
    std::string path = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/NonClosedEmptySet/";
    std::string extension = ".txt";

    unsigned int tests_nb = 3;
    std::vector<std::string> filenames = {"input", "inter", "output"};

    MaierTester tester;

    std::cout << "Test Case: Non-closed Emptyset" << std::endl << std::endl;
    tester.TestingSequence(path, extension, filenames, tests_nb);
}


/// \brief Test of "standard" basis.
///
/// Input is a basis randomly generated (reduced).
TEST_CASE("Maier Algorithm: some standard tests")
{
    std::string path = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/Standard/";
    std::string extension = ".txt";

    unsigned int tests_nb = 3;
    std::vector<std::string> filenames = {"input", "inter", "output"};

    MaierTester tester;

    std::cout << "Test Case: Standard cases" << std::endl << std::endl;
    tester.TestingSequence(path, extension, filenames, tests_nb);
}


/// \brief Test of non-reduced form.
///
/// The input is a basis with implications A --> B, such that A n B != {} (possibly).
/// The algorithm is expected to work the same way as usual.
TEST_CASE("Maier Algorithm: Non-reduced form")
{
    std::string path = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/NonReduced/";
    std::string extension = ".txt";

    unsigned int tests_nb = 1;
    std::vector<std::string> filenames = {"input", "inter", "output"};

    MaierTester tester;

    std::cout << "Test Case: Non-Reduced cases" << std::endl << std::endl;
    tester.TestingSequence(path, extension, filenames, tests_nb);
}


