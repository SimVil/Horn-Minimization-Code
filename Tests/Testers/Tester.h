/// \file Tester.h
/// \author Simon Vilmin
/// \date 2018
///
/// \brief contains interface definition for unit testing.
///
/// The file contains the definition of an interface Tester, with general member functions. The aim is consequently
/// to provide various implementation corresponding to tests of horn minimization algorithm. See the documentation of
/// Tester for more information. We use catch as a unit testing framework.

#ifndef ALGORITHMS_TESTER_H
#define ALGORITHMS_TESTER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../catch.h"
#include "../../CanonicalBasis/fca_datastructures.h"
#include "../../CanonicalBasis/test_functions.h"

/// \class interface for testing horn minimization algorithm.
///
/// This interface provides functions able to read implication text files. See some test files for formatting. The
/// difference between various algorithm requiring test classes implementing this partial interface, is the number of
/// test to be performed. The Tester class can read a sequence of files, and automatically (read loop) treat them.
/// For instance, testing the algorithm by Maier can be done in two steps:
///     - testing non-redundancy after step 1
///     - testing minimality
/// whereas the algorithm by Berczi can somehow be tested only on its result, the DG basis. Thus, the sequences of file
/// to be read are not of the same size. This is the point of the virtual function BuildingFunction.
class Tester {
public:

    /// \brief *virtual* fills a vector of experimental results to be tested against theoretical expectations.
    ///
    /// This member function fills a vector \c buff of implication bases obtained by running algorithm of minimization.
    /// Because we would like to test various results for each algorithm, this method is virtual and must be implemented
    /// properly for each algorithm to match its specific needs. The vector filled in this method will be used in \c
    /// TestingSequence to perform equivalence tests against theoretical expectations.
    ///
    /// \param buff vector to be fulfilled of resulting implication bases.
    /// \param L input of the minimization algorithm to be tested.
    /// \param sigma attribute set. needed to perform conversion from bitset to attributes symbols.
    virtual void BuildingSequence(std::vector<std::vector<FCA::Implication>> &buff,
                                  const std::vector<FCA::ImplicationInd> &L,
                                  const std::vector<FCA::Attribute> &sigma) = 0;

    /// \brief Performs a list of tests out of test files.
    ///
    /// Theoretical principle: the function reads a text file containing an input implication base. Then, this base
    /// is converted to bitsets implications and used as the input of some minimization algorithm through \c
    /// BuildingSequence. The results produced by the algorithm are compared to expected results read in other text
    /// files.
    ///
    /// \param path place to find files to read.
    /// \param extension extension of testing files.
    /// \param filenames
    /// \param tests_nb number of equivalence tests to be done.
    void TestingSequence(const std::string &path,
                         const std::string &extension,
                         const std::vector<std::string> &filenames,
                         const unsigned &tests_nb);

    /// \brief Reads a sequence of test files.
    ///
    /// \param filenames files to read (input basis, expected ouput, etc).
    /// \param basis read implications (attribute form).
    /// \param sigma attribute set.
    static void ReadingSequence(std::vector<std::string> &filenames,
                                std::vector<std::vector<FCA::Implication>> &basis,
                                std::vector<FCA::Attribute> &sigma);

    /// \brief General testing procedure.
    ///
    /// Runs the TestingSequence for several sequences of file under test cases separation. For each test case of
    /// implication base, various sequences of files are treated. test case can be contradictions, non reduced form,
    /// and so forth. In fact, we limit our test to "algorithmic" test cases, admitting that files are well-written.
    /// This choice is based upon the experimental/research and not UI-designed purpose of the code.
    ///
    /// \param testcases list of test cases and number of tests per test case.
    /// \param filenames list of test files names.
    /// \param root directory in which search files.
    /// \param extension extension of test files (usually txt).
    void Test(const std::vector<std::pair<std::string, unsigned int>> &testcases,
              const std::vector<std::string> &filenames,
              const std::string &root,
              const std::string &extension = ".txt"
            );

protected:

    /// \brief read an implication base into a text file.
    ///
    /// \param filename
    /// \param sigma attribute set (to be read also in the file).
    /// \param basis basis read in the file.
    static void ReadImplicationFile(const std::string &filename, std::vector<FCA::Attribute> &sigma,
                                    std::vector<FCA::Implication> &basis);

    /// \brief special case of empty strings.
    ///
    /// Because tests allows contradiction and so forth, we may obtain a "" attribute when parsing
    /// implications. This must be converted into an emptyset, and not to a non-existent attribute.
    ///
    /// \param set set from which delete the element.
    static void HandleEmpty(std::vector<FCA::Attribute> &set);

};


#endif //ALGORITHMS_TESTER_H
