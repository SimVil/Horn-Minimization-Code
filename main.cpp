#define CATCH_CONFIG_MAIN

#include <iostream>
#include <vector>
#include "catch.h"
#include "CanonicalBasis/test_functions.h"
#include "CanonicalBasis/graph_FDgraph.h"
#include "CanonicalBasis/horn_maier.h"
#include "CanonicalBasis/horn_berczi.h"
#include "Tests/Testers/Tester.h"
#include <ctime>
// #include <boost/timer/timer.hpp>

// #include <gperftools/profiler.h>


TEST_CASE("Main") {
//int main(int, char **) {

    unsigned attrNum = 10;
    unsigned implNum = 20;

    srand((unsigned) time(nullptr));
    std::vector<FCA::Attribute> sigma_s = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
    std::vector<FCA::Implication> L_s(implNum);

    FCA::BitSet premise(attrNum), conclusion(attrNum);
    FCA::BitSet sigma;
    std::vector<FCA::ImplicationInd> L;
    std::vector<FCA::ImplicationInd> Lbis;

//    std::cout << (1 << attrNum) - 1 << std::endl;
//
//    for (unsigned i = 0; i < implNum; ++i)
//    {
//        do {
//            conclusion = GetRandomBitSet(attrNum, 0.2);
//        } while (conclusion.none() || conclusion.count() == (unsigned) (1 << attrNum) - 1);
//
//        do {
//            premise = GetRandomBitSet(attrNum, 0.5) - conclusion;
//        } while (premise.none());
//
//        L.emplace_back(FCA::ImplicationInd(premise, conclusion));
//
//    }



    Tester::ReadImplicationFile("D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/Standard/input_4.txt", sigma_s, L_s);
    FCA::Convert(sigma_s, sigma_s, L_s, sigma, L);

    GRAPH::FDGraph g(L);
    g.Convert(Lbis);

    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));

    std::cout << "Redundancy Watch " << std::endl;

    g.Closure();
    g.RedundancyElimination();
    g.Convert(Lbis);
    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));

    Lbis = HORN::redundancyElimination(L);
    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));

    std::cout << std::endl << " ---------------------------" << std::endl;
    std::cout << "Final Results (Ausiello, Maier, Berczi)" << std::endl << std::endl;

    std::cout << "Ausiello Result" << std::endl;
    g.SuperfluousnessElimination();
    g.Convert(Lbis);
    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));

    std::cout << std::endl << "Maier Result" << std::endl;
    Lbis = HORN::MaierMinimization(L);
    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));

    std::cout << std::endl << "Berczi Result" << std::endl;
    Lbis = HORN::BercziMinimization(L);
    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));

}