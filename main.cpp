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
#include "Tests/Testers/GridTester.h"
#include <ctime>
#include <boost/timer/timer.hpp>

// #include <gperftools/profiler.h>

int main(int, char **){
    std::pair<unsigned, unsigned> p = {1, 4};
    std::pair<unsigned, unsigned> q = {0, 2};
    std::pair<unsigned, unsigned> r = {8, 9};
    std::vector<std::string> param {"x", "y", "z", "a"};
    GridTester tester(param, q);
    tester.GridSearch(param);
    tester.setParam(std::vector<std::string> {"y", "a"}, q);
    tester.setParam(std::vector<std::string> {"x", "z"}, r);
    tester.GridSearch(param);
    return 0;
}

