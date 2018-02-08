#include <iostream>
#include "../CanonicalBasis/horn_maier.h"
#include "../CanonicalBasis/test_functions.h"
#include "../Tests/Reader.h"

int main(int, char **) {
    std::string filename("D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/basis_3.txt");
    std::vector<FCA::Attribute> sigma_string;
    std::vector<FCA::Implication> basis_string;
    FCA::BitSet sigma;
    std::vector<FCA::ImplicationInd> basis;

    Reader::ReadImplicationFile(filename, sigma_string, basis_string);
    FCA::Convert(sigma_string, sigma_string, basis_string, sigma, basis);

    std::vector<FCA::ImplicationInd> minBasis = HORN::MaierMinimization(basis);

    std::vector<FCA::Implication> minbasis_string = FCA::Convert(minBasis, sigma_string);
    PrintImplications(std::cout, minbasis_string);
    return 0;
}