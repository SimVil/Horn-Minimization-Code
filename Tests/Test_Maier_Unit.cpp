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


TEST_CASE("Maier Algorithm: Non-empty basis") {
    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/";
    MaierTester tester;

    std::vector<std::pair<std::string, unsigned >> testcases = {
            {"Contradictions",    2},
            {"NonClosedEmptySet", 3},
            {"NonReduced",        1},
            {"Standard",          3}};

    std::vector<std::string> filenames = {"input", "inter", "output"};

    tester.Test(testcases, filenames, root);
}