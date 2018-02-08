//
// Created by Simon on 06/02/2018.
//

#include "Reader.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

void Reader::HandleEmpty(std::vector<FCA::Attribute> &set) {
    if (set.size() == 1 && set[0].empty())
    {
        set.pop_back();
    }
}

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
        boost::split(sigma, line, boost::is_any_of(" "));

        while(std::getline(file, line))
        {
            boost::split(leftRight, line, boost::is_any_of(">"));
            boost::trim(leftRight[0]);
            boost::trim(leftRight[1]);
            boost::split(premise, leftRight[0], boost::is_any_of(" "));
            boost::split(conclusion, leftRight[1], boost::is_any_of("  "));
            Reader::HandleEmpty(premise);
            Reader::HandleEmpty(conclusion);
            basis.emplace_back(FCA::Implication(premise, conclusion));
        }

        file.close();
    } else
    {
        std::cout << "Reader::ReadImplicationFile - Unable to open file" << std::endl;
    }




}

void Reader::FileSequenceHandler(std::vector<std::string> &filenames, std::vector<std::vector<FCA::Implication>> &basis,
                                std::vector<FCA::Attribute> &sigma)
{

    unsigned int file_nb = filenames.size();

    if(file_nb != basis.size()){
        throw ("Reader::FileSequenceHandler: we need as much basis as files to read.");
    }

    for(int i = 0; i < file_nb; ++i)
    {
        Reader::ReadImplicationFile(filenames[i], sigma, basis[i]);
    }
}
