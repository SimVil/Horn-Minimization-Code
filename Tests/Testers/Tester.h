//
// Created by Simon on 09/02/2018.
//

#ifndef ALGORITHMS_TESTER_H
#define ALGORITHMS_TESTER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../catch.h"
#include "../../CanonicalBasis/fca_datastructures.h"
#include "../../CanonicalBasis/test_functions.h"

class Tester {
public:
    virtual void BuildingSequence(std::vector<std::vector<FCA::Implication>> &buff,
                                  const std::vector<FCA::ImplicationInd> &L,
                                  const std::vector<FCA::Attribute> &sigma) = 0;

    void TestingSequence(const std::string &path, const std::string &extension, const std::vector<std::string> &filenames,
                         const unsigned &tests_nb);

    static void ReadingSequence(std::vector<std::string> &filenames, std::vector<std::vector<FCA::Implication>> &basis,
                                std::vector<FCA::Attribute> &sigma);

protected:

    static void ReadImplicationFile(const std::string &filename, std::vector<FCA::Attribute> &sigma,
                                    std::vector<FCA::Implication> &basis);

    static void HandleEmpty(std::vector<FCA::Attribute> &set);



};


#endif //ALGORITHMS_TESTER_H
