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
    std::map<std::string, unsigned> p;
    std::map<std::string, double> results;

    boost::timer::auto_cpu_timer t;

    p["implNum"] = 5;
    p["attrNum"] = 4;
    p["gen"] = 50;
    p["repeat"] = 1;
    std::vector<std::string> algos = {"Maier"};
    tester.PerformTestCase(p, algos, results);
    return 0;
}

