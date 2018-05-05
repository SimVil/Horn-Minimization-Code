#define CATCH_CONFIG_MAIN

#include <iostream>
#include <vector>
#include "CanonicalBasis/horn_minimization_algo.h"
#include "Tests/Tools/ImplicationTools.h"
#include "Tests/Testers/GridTester.h"

//int main(int, char **){
//    std::string root = "D:/Documents/Courses/Master Thesis/Code/DataSets/";
//    std::string filename = root + "vote.cxt";
//    std::string name = "vote";
//
//    ImplicationTools::getRealBasis(filename, name);
//
//    return 0;
//}

void small_test(){
    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/BitImplicationFiles/";
    std::string filename = root + "imp_duquenne.imp";

    theory L, Lc;
    ImplicationTools::ReadFile(filename, L);
    HORN::BercziMinimization<FCA::LinClosure>(L, Lc);

}



int main(int, char **) {
    srand((unsigned) time(nullptr));
    GridTester tester;
    std::vector<unsigned> impl_space = {100};
    std::pair<unsigned, unsigned> attr_space(2000, 50000);
    std::vector<unsigned> gen_space = {100};
    std::vector<unsigned> rep_space = {1};

    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/";
    std::string filename = "Berczi_ClosureOperators_100impl_p1.csv";

    tester.setParam("implNum", impl_space);
    tester.setParam("attrNum", attr_space, [](int){return 2000;});
    tester.setParam("gen", gen_space);
    tester.setParam("repeat", rep_space);

    std::vector<std::string> algos = {"BercziLin", "BercziClo"}; //, "MinCover", "Berczi"};

    boost::timer::auto_cpu_timer t;
    tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos, root + filename);

    return 0;
}

//int main(int, char **){
//    GridTester tester;
//    std::vector<unsigned> attr_space = {5};
//    std::vector<unsigned> impl_space = {5};
//    std::vector<unsigned> gen_space = {100};
//    std::vector<unsigned> rep_space = {1};
//
//    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/";
//    std::string filename = "Duquenne_Debug.csv";
//
//    tester.setParam("implNum", impl_space);
//    tester.setParam("attrNum", attr_space);
//    tester.setParam("gen", gen_space);
//    tester.setParam("repeat", rep_space);
//
//    boost::timer::auto_cpu_timer t;
//
//    std::vector<std::string> algos = {"Duquenne"};
//    tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos, root + filename);
//
//    return 0;
//}
