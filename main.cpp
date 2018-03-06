#define CATCH_CONFIG_MAIN

#include <iostream>
#include <vector>
#include "catch.h"
#include "CanonicalBasis/test_functions.h"
#include "CanonicalBasis/graph_FDgraph.h"
#include "Tests/Testers/Tester.h"
#include <ctime>
// #include <boost/timer/timer.hpp>

// #include <gperftools/profiler.h>


TEST_CASE("Main") {
//int main(int, char **) {

    std::vector<FCA::Attribute> sigma_s;
    std::vector<FCA::Implication> L_s;

    FCA::BitSet sigma;
    std::vector<FCA::ImplicationInd> L;
    std::vector<FCA::ImplicationInd> Lbis;

    Tester::ReadImplicationFile("/home/simon/share/Algorithms/Tests/Standard/input_4.txt", sigma_s, L_s);
    FCA::Convert(sigma_s, sigma_s, L_s, sigma, L);

    GRAPH::FDGraph g(L);
    g.Convert(Lbis);

    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));

    g.Closure();
    g.RedundancyElimination();
    g.Convert(Lbis, "full");
    PrintImplications(std::cout, FCA::Convert(Lbis, sigma_s));

}