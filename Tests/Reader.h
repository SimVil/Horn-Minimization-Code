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

    static void HandleEmpty(std::vector<FCA::Attribute> &set);

    static void FileSequenceHandler(std::vector<std::string> &filenames, std::vector<std::vector<FCA::Implication>> &basis,
                                       std::vector<FCA::Attribute> &sigma);
};


#endif //ALGORITHMS_READER_H
