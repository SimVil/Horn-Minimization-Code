//
// Created by Simon on 09/02/2018.
//

#include "Tester.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>


/// Performs a list of tests out of test files.
void Tester::TestingSequence(const std::string &path, const std::string &extension,
                             const std::vector<std::string> &filenames, const unsigned &tests_nb)
{
    unsigned long seq_len = filenames.size();
    std::vector<std::string> buffer(seq_len);
    std::vector<std::vector<FCA::Implication>> basis(seq_len);
    std::vector<FCA::Attribute> sigma_s; // attribute set
    std::vector<FCA::ImplicationInd> L; // input basis
    std::vector<std::vector<FCA::Implication>> buffL; // result buffers
    FCA::BitSet sigma; // bitset attribute set

    for(int i = 1; i <= tests_nb; ++i)
    {
        // formatting file names
        for(int j = 0; j < seq_len; ++j)
        {
            buffer[j] = path;
            buffer[j] += filenames[j];
            buffer[j] += "_";
            buffer[j] += std::to_string(i);
            buffer[j] += extension;
        }

        // read the files defined by buffer
        Tester::ReadingSequence(buffer, basis, sigma_s);
        FCA::Convert(sigma_s, sigma_s, basis[0], sigma, L);

        BuildingSequence(buffL, L, sigma_s);

        // removing the input file, we must have exactly seq_len - 1 results to test.
        REQUIRE(buffL.size() == seq_len - 1);

        // preforming equivalence test between results and expectations
        for(int k = 0; k < buffL.size(); ++k)
        {
            REQUIRE(IsVectorOfImplicationsIdentical(buffL[k], basis[k + 1]));
            WARN("File: " << i);
        }

        for(int j = 0; j < seq_len; ++j)
        {
            basis[j].clear();
        }

        L.clear();
        buffL.clear();

    }
}

/// special case of empty strings.
void Tester::HandleEmpty(std::vector<FCA::Attribute> &set) {
    if (set.size() == 1 && set[0].empty())
    {
        set.pop_back();
    }
}

/// read an implication base into a text file.
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
        // read attribute set
        std::getline(file, line);
        boost::trim(line);
        boost::split(sigma, line, boost::is_any_of(" "));

        // read implications in the form: a b c > d
        // we remove spaces and \r of strings to avoid noisy attributes
        // then we convert the strings to implications
        while(std::getline(file, line))
        {
            boost::split(leftRight, line, boost::is_any_of(">"));
            boost::trim_if(leftRight[0], boost::is_any_of(" \r"));
            boost::trim_if(leftRight[1], boost::is_any_of(" \r"));
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


/// Reads a sequence of test files.
void Tester::ReadingSequence(std::vector<std::string> &filenames, std::vector<std::vector<FCA::Implication>> &basis,
                             std::vector<FCA::Attribute> &sigma)
{

    unsigned long file_nb = filenames.size();

    if(file_nb != basis.size()){
        throw ("Tester::ReadingSequence: we need as much basis as files to read.");
    }

    for(int i = 0; i < file_nb; ++i)
    {
        Tester::ReadImplicationFile(filenames[i], sigma, basis[i]);
    }
}


/// General testing procedure.
void Tester::Test(const std::vector<std::pair<std::string, unsigned int>> &testcases,
                  const std::vector<std::string> &filenames,
                  const std::string &root,
                  const std::string &extension)
{
    std::string path;
    unsigned long tests_nb = testcases.size();
    for(int i = 0; i < tests_nb; ++i)
    {
        SECTION(testcases[i].first)
        {
            std::cout << "---- Beginning case: " << testcases[i].first << std::endl;
            path = root;
            path += testcases[i].first;
            path += "/";
            TestingSequence(path, extension, filenames, testcases[i].second);
            std::cout << "---- Ending case. ----" << std::endl;
        }
    }
}
