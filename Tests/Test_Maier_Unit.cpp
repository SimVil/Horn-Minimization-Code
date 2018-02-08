/// \file Test_Maier_Unit.cpp


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch.h"
#include "../CanonicalBasis/test_functions.h"
#include "../CanonicalBasis/horn_maier.h"
#include "Reader.h"



TEST_CASE("FCA::LinClosure::Apply: Simple tests")
{
    std::string filename("D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/basis_1.txt");
    std::vector<FCA::Attribute> sigma_string;
    std::vector<FCA::Implication> basis_string;
    FCA::BitSet sigma;
    std::vector<FCA::ImplicationInd> basis;

    Reader::ReadImplicationFile(filename, sigma_string, basis_string);
    FCA::Convert(sigma_string, sigma_string, basis_string, sigma, basis);

    SECTION("Check That LinClosure computes the right equivalent bitset of a given implication")
    {
        FCA::BitSet equivalent(basis.size());
        FCA::BitSet expected(basis.size());
        FCA::BitSet buff(sigma.size());
        equivalent.reset();
        expected.reset();

        expected.set(0);
        expected.set(1);

        FCA::LinClosure::Apply(basis.front().Premise(), basis, buff, 0, &equivalent);

        std::vector<FCA::Attribute> closed = FCA::Convert(buff, sigma_string);
        std::vector<FCA::Attribute> initial = FCA::Convert(basis.front().Premise(), sigma_string);

        for(const FCA::Attribute &s : initial)
        {
            std::cout << s;
        }

        std::cout << std::endl;

        for(const FCA::Attribute &s : closed)
        {
            std::cout << s;
        }

        std::cout << std::endl;
        REQUIRE(equivalent == expected);
    }
}

