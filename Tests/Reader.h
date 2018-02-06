//
// Created by Simon on 06/02/2018.
//

#ifndef ALGORITHMS_READER_H
#define ALGORITHMS_READER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../CanonicalBasis/fca_datastructures.h"

class Reader
{
public:
    static void ReadImplicationFile(std::string filename, std::vector<FCA::Attribute> &sigma,
        std::vector<FCA::Implication> &basis);
};


#endif //ALGORITHMS_READER_H
