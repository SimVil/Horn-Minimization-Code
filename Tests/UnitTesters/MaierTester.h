//
// Created by Simon on 09/02/2018.
//

#ifndef ALGORITHMS_MAIERTESTER_H
#define ALGORITHMS_MAIERTESTER_H

#include "Tester.h"
#include "../../CanonicalBasis/horn_maier.h"

class MaierTester: public Tester {
public:
    void BuildingSequence(std::vector<std::vector<FCA::Implication>> &buff,
                                  const std::vector<FCA::ImplicationInd> &L,
                          const std::vector<FCA::Attribute> &sigma) override;
};


#endif //ALGORITHMS_MAIERTESTER_H
