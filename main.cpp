#define CATCH_CONFIG_MAIN

#include <iostream>
#include <vector>
#include "CanonicalBasis/test_functions.h"
#include "CanonicalBasis/horn_maier.h"
#include "CanonicalBasis/horn_berczi.h"
#include "CanonicalBasis/horn_lectic.h"
#include "CanonicalBasis/horn_shock.h"
#include "CanonicalBasis/fca_minimization.h"
#include "Tests/Tools/ImplicationTools.h"
#include <ctime>
#include <boost/timer/timer.hpp>

// #include <gperftools/profiler.h>

int main(int, char **){
    std::vector<FCA::ImplicationInd> L, Lbis;

    srand((unsigned) time(nullptr));

    ImplicationTools::GenerateTheory(L, 10, 10);
    PrintImplications(std::cout, FCA::Convert(L, {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"}));

    FCA::BitSet emptyset(10);
    ImplicationTools::Reduce(L);
    ImplicationTools::CloseSet(emptyset, L);
    PrintImplications(std::cout, FCA::Convert(L, {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"}));

    Lbis = HORN::MaierMinimization(L);
    PrintImplications(std::cout, FCA::Convert(Lbis, {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"}));

    ImplicationTools::ExpandTheory(Lbis, 1.);
    PrintImplications(std::cout, FCA::Convert(Lbis, {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"}));
    Lbis = HORN::MaierMinimization(Lbis);
    PrintImplications(std::cout, FCA::Convert(Lbis, {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"}));

    std::cout << std::endl;
    return 0;
}


//TEST_CASE("Main") {
////int main(int, char **) {
//
//    unsigned attrNum = 10;
//    unsigned implNum = 10;
//
//    srand((unsigned) time(nullptr));
//    std::vector<FCA::Attribute> sigma_s = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
//    std::vector<FCA::Implication> L_s;
//
//    FCA::BitSet premise(attrNum), conclusion(attrNum);
//    FCA::BitSet sigma;
//    std::vector<FCA::ImplicationInd> L;
//    std::vector<FCA::ImplicationInd> Lbis;
//
//    std::cout << (1 << attrNum) - 1 << std::endl;
////
////    for (unsigned i = 0; i < implNum; ++i)
////    {
////        do {
////            conclusion = GetRandomBitSet(attrNum, 0.2);
////        } while (conclusion.none() || conclusion.count() == (unsigned) (1 << attrNum) - 1);
////
////        do {
////            premise = GetRandomBitSet(attrNum, 0.5) - conclusion;
////        } while (premise.none());
////
////        L.emplace_back(FCA::ImplicationInd(premise, conclusion));
////
////    }
//
//    boost::timer::auto_cpu_timer t;
//
//
//    Tester::ReadImplicationFile("D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/Standard/input_3.txt", sigma_s, L_s);
//    FCA::Convert(sigma_s, sigma_s, L_s, sigma, L);
//
//    PrintImplications(std::cout, FCA::Convert(L, sigma_s));
//
//    std::cout << "Redundancy Watch " << std::endl;
//
//    Lbis = HORN::redundancyElimination(L);
//    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));
//
//    std::cout << std::endl << " ---------------------------" << std::endl;
//    std::cout << "Final Results (Lectic, Maier, Berczi, Minimal Cover)" << std::endl << std::endl;
//
//    std::cout << std::endl << "Maier Result" << std::endl;
//    Lbis = HORN::MaierMinimization(L);
//    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));
//
//    Lbis.clear();
//
//    std::cout << std::endl << "Berczi Result" << std::endl;
//    HORN::BercziMinimization(L, Lbis);
//    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));
//
//    std::cout << std::endl << "Shock result" << std::endl;
//    HORN::ShockMinimization(L, Lbis);
//    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));
//
//    Lbis.clear();
//    std::cout << std::endl << "Minimal Cover Result" << std::endl;
//    Lbis = FCA::MinimalCover(L);
//    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));
//
//}