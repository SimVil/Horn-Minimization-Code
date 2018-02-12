//
// Created by Simon on 09/02/2018.
//

#ifndef ALGORITHMS_BERCZITESTER_H
#define ALGORITHMS_BERCZITESTER_H

#include "Tester.h"
#include "../../CanonicalBasis/horn_berczi.h"

class BercziTester: public Tester {
public:
    void BuildingSequence(std::vector<std::vector<FCA::Implication>> &buff,
                          const std::vector<FCA::ImplicationInd> &L,
                          const std::vector<FCA::Attribute> &sigma) override;
};


#endif //ALGORITHMS_BERCZITESTER_H
