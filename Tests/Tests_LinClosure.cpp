
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch.h"
#include "../CanonicalBasis/fca_linclosure.h"
#include "../CanonicalBasis/test_functions.h"
#include "Reader.h"



TEST_CASE("FCA::LinClosure::Apply: Simple tests")
{
    std::string filename("D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/basis_1.txt");
    std::vector<FCA::Attribute> sigma;
    std::vector<FCA::Implication> basis_string;

    Reader::ReadImplicationFile(filename, sigma, basis_string);

    SECTION("Check for correct closure")
    {
        PrintImplications(std::cout, basis_string);
    }
}

