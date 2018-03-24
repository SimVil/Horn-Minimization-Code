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
    srand((unsigned) time(nullptr));
    GridTester tester;
    std::pair<unsigned, unsigned> size_space(100, 1000);
    std::pair<unsigned, unsigned> gen_space(20, 20);
    std::pair<unsigned, unsigned> rep_space(5, 5);

    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/";
    std::string filename = "log_2.csv";

    tester.setParam(std::vector<std::string> {"implNum", "attrNum"}, size_space, [](int){return 25;});
    tester.setParam("gen", gen_space);
    tester.setParam("repeat", rep_space);

    std::vector<std::string> algos = {"Maier", "MinCover", "Berczi"};
    tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos, root + filename);

    return 0;
}

