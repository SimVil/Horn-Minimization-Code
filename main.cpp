#include <iostream>
#include "CanonicalBasis/fca_closure_operators.h"
#include "CanonicalBasis/fca_datastructures.h"
#include "CanonicalBasis/fca_definitions.h"
#include "CanonicalBasis/test_functions.h"

int main(int, char **) {
    int sigma_size = 10;
    int basis_size = 5;
    std::vector<FCA::Attribute> sigma = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
    std::vector<FCA::ImplicationInd> L(5);


    for (FCA::Attribute i : sigma){
        std::cout << i << std::endl;
    }

    std::vector<FCA::ImplicationInd> L(5);
    for (FCA::ImplicationInd i : L){

    }

    return 0;
}