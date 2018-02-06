//
// Created by Simon on 06/02/2018.
//

#include "Reader.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

void Reader::ReadImplicationFile(std::string filename, std::vector<FCA::Attribute> &sigma,
                                        std::vector<FCA::Implication> &basis)
{
    std::ifstream file (filename);
    std::string line;
    std::vector<std::string> leftRight(2);
    std::vector<FCA::Attribute> premise;
    std::vector<FCA::Attribute> conclusion;

    if (file.is_open())
    {
        std::getline(file, line);
        boost::split(sigma, line, boost::is_any_of(", "));

        while(std::getline(file, line))
        {
            boost::split(leftRight, line, boost::is_any_of(" | "));
            boost::split(premise, leftRight[0], boost::is_any_of(", "));
            boost::split(conclusion, leftRight[1], boost::is_any_of(", "));
            basis.emplace_back(FCA::Implication(premise, conclusion));
        }

        file.close();
    } else
    {
        std::cout << "Reader::ReadImplicationFile - Unable to open file" << std::endl;
    }

}