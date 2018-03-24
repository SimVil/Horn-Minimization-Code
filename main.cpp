#define CATCH_CONFIG_MAIN

#include <iostream>
#include <vector>
#include "CanonicalBasis/horn_minimization_algo.h"
#include "Tests/Tools/ImplicationTools.h"
#include "Tests/Testers/GridTester.h"
#include <ctime>
#include <boost/timer/timer.hpp>

// #include <gperftools/profiler.h>

int main(int, char **){
    GridTester tester;
    std::pair<unsigned, unsigned> size_space(10, 20);
    std::pair<unsigned, unsigned> gen_space(20, 20);
    std::pair<unsigned, unsigned> rep_space(20, 20);

    tester.setParam(std::vector<std::string> {"implNum", "attrNum"}, size_space);
    tester.setParam("gen", gen_space);
    tester.setParam("repeat", rep_space);

    std::vector<std::string> algos = {"Maier", "MinCover"};
    tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos);

    return 0;
}

