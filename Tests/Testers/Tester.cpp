//
// Created by Simon on 09/02/2018.
//

#include "Tester.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>


void Tester::TestingSequence(const std::string &path, const std::string &extension,
                             const std::vector<std::string> &filenames, const unsigned &tests_nb)
{
    unsigned int seq_len = filenames.size();
    std::vector<std::string> buffer(seq_len);
    std::vector<std::vector<FCA::Implication>> basis(seq_len);
    std::vector<FCA::Attribute> sigma_s;
    std::vector<FCA::ImplicationInd> L;
    std::vector<FCA::ImplicationInd> minL;
    std::vector<std::vector<FCA::Implication>> buffL;
    FCA::BitSet sigma;

    for(int i = 1; i <= tests_nb; ++i)
    {
        for(int j = 0; j < seq_len; ++j)
        {
            buffer[j] = path;
            buffer[j] += filenames[j];
            buffer[j] += "_";
            buffer[j] += std::to_string(i);
            buffer[j] += extension;
        }

        Tester::ReadingSequence(buffer, basis, sigma_s);
        FCA::Convert(sigma_s, sigma_s, basis[0], sigma, L);

        BuildingSequence(buffL, L, sigma_s);

        REQUIRE(buffL.size() == seq_len - 1);

        for(int k = 0; k < buffL.size(); ++k)
        {
            REQUIRE(IsVectorOfImplicationsIdentical(buffL[k], basis[k + 1]));
        }

        for(int j = 0; j < seq_len; ++j)
        {
            basis[j].clear();
        }

        L.clear();
        buffL.clear();

    }
}


void Tester::HandleEmpty(std::vector<FCA::Attribute> &set) {
    if (set.size() == 1 && set[0].empty())
    {
        set.pop_back();
    }
}

void Tester::ReadImplicationFile(const std::string &filename, std::vector<FCA::Attribute> &sigma,
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
            Tester::HandleEmpty(premise);
            Tester::HandleEmpty(conclusion);
            basis.emplace_back(FCA::Implication(premise, conclusion));
        }

        file.close();
    } else
    {
        std::cout << "Reader::ReadImplicationFile - Unable to open file" << std::endl;
    }




}

void Tester::ReadingSequence(std::vector<std::string> &filenames, std::vector<std::vector<FCA::Implication>> &basis,
                             std::vector<FCA::Attribute> &sigma)
{

    unsigned int file_nb = filenames.size();

    if(file_nb != basis.size()){
        throw ("Tester::ReadingSequence: we need as much basis as files to read.");
    }

    for(int i = 0; i < file_nb; ++i)
    {
        Tester::ReadImplicationFile(filenames[i], sigma, basis[i]);
    }
}