/// \file Test_Berczi_Unit.cpp
/// \author Simon Vilmin.
/// \date 2018
///
/// \brief Contains a basic set of unit test cases to check correctness of Berczi Algorithm for minimization. Made with
/// catch2.
///
/// The code is not expected to be used in any commercial software. Its aim is to test research algorithms. As
/// a consequence, we are not going to test validity of input/output formatting. We want to test correctness of
/// algorithm, thus we will focus on special cases of basis more than all the cases of wrong manipulation of the
/// code leading to a crash.
/// Tests are:
///     - empty basis
///     - ...
/// Tests will split over various parts of the algorithm (see horn_berczi.h for more details).

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch.h"
#include "../CanonicalBasis/horn_berczi.h"
#include "Testers/BercziTester.h"
#include <gperftools/profiler.h>


/// \brief Test of empty basis.
///
/// We give as an input to the test an empty basis L. Quite easily, the algorithm should stop almost immediately:
///     - Input: L = {}
///     - Expected output:
///         -# irredundancy: Lc = {}
///         -# maier minimization:  Lc = {}
TEST_CASE("Berczi Algorithm: empty basis")
{
    std::vector<FCA::ImplicationInd> L;
    std::vector<FCA::ImplicationInd> minL = HORN::BercziMinimization(L);
    std::vector<FCA::BitSet> matrix;

    REQUIRE(minL.empty());
}



TEST_CASE("Berczi Algorithm: Non-empty basis")
{

    ProfilerStart("/home/simon/share/Algorithms/out.prof");
    std::string root = "/home/simon/share/Algorithms/Tests/";
    BercziTester tester;

    std::vector<std::pair<std::string, unsigned >> testcases = {
            {"Contradictions",    2},
            {"NonClosedEmptySet", 3},
            {"NonReduced",        1},
            {"Standard",          3}};

    std::vector<std::string> filenames = {"input", "DG"};

    for(int i = 0; i < 1000; i++)
    {
        tester.Test(testcases, filenames, root);
    }

    ProfilerStop();
}

