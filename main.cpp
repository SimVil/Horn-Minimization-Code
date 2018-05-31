#define CATCH_CONFIG_MAIN

#include <iostream>
#include <vector>
#include "CanonicalBasis/horn_minimization_algo.h"
#include "Tests/Tools/ImplicationTools.h"
#include "Tests/Testers/GridTester.h"

void getdata(){
    std::string root = "D:/Documents/Courses/Master Thesis/Code/DataSets/";
    std::string filename = root + "post-operative.cxt";
    std::string name = "post-operative";

    ImplicationTools::getRealBasis(filename, name);

}

void new_min(){
    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/RealDatasets/";
    std::vector<std::string> ds = {"breast-cancer", "breast-w", "flare", "post-operative", "SPECT", "vote" ,"zoo"};

    theory L, Lc;
    for(auto &name : ds){
        ImplicationTools::ReadFile(root + name + "_proper.imp", L);
        HORN::MaierMinimization<FCA::Closure, FCA::Closure>(L, Lc);
        ImplicationTools::WriteFile(root + name + "_min2.imp", Lc);
    }

}

void small_test(){
    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/BitImplicationFiles/";
    std::string filename = root + "usual.imp";

    theory L, Lc;
    ImplicationTools::ReadFile(filename, L);
    HORN::MaierMinimization<FCA::Closure, FCA::LinClosure>(L, Lc);
    std::cout << Lc.size() << std::endl;

}

void equivalence_test(){
    theory L, Lm, Ld;

    for(int i = 0; i < 1000; i++){
        ImplicationTools::GenerateTheory(L, 200, 200);
        HORN::DuquenneMinimization<FCA::Closure>(L, Ld);
        HORN::MaierMinimization<FCA::Closure, FCA::LinClosure>(L, Lm);
        if (!ImplicationTools::AreEquivalent(Ld, Lm)){
            std::cout << i << std::endl;
        }

    }
}


void worst_test(){
    srand((unsigned) time(nullptr));
    GridTester tester;
    std::vector<unsigned> attr_space = {100};
    std::vector<unsigned> impl_space = {100};
    std::vector<unsigned> gen_space = {2000};
    std::vector<unsigned> rep_space = {1};

    tester.setParam("gen", gen_space);
    tester.setParam("repeat", rep_space);

    std::vector<std::string> algos = {"MinCoverClo", "MaierClo", "Duquenne"};

    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/Real/MinimalTests/";
    std::string imp_root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/RealDatasets/";

    std::vector<std::string> testname = {"SPECT", "flare", "post-operative", "vote", "zoo", "breast-cancer", "breast-w"};
    std::vector<std::string> basistype = {"DG"};
    std::string ithname;

    theory L;

    for (auto &n : testname){
        for(auto &t : basistype){
            ithname = n;
            ithname += "_" + t;
            ImplicationTools::ReadFile(imp_root + ithname + ".imp", L);
            attr_space = {(unsigned) L.front().Premise().size()};
            impl_space = {(unsigned) L.size()};
            tester.setParam("attrNum", attr_space);
            tester.setParam("implNum", impl_space);
            tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos, root + ithname + "-2.csv");

        }
    }

}

//int main(int, char **){
//    new_min();
//    return 0;
//}

int main(int, char **) {
    srand((unsigned) time(nullptr));
    GridTester tester;
    std::vector<unsigned> attr_space = {100};
    std::pair<unsigned, unsigned> impl_space(100, 10000);
    std::vector<unsigned> gen_space = {500};
    std::vector<unsigned> rep_space = {1};

    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/Random/";
    std::string filename = "All_100attr_500rep.csv";

    tester.setParam("attrNum", attr_space);
    tester.setParam("implNum", impl_space, [](int){return 100;});
    tester.setParam("gen", gen_space);
    tester.setParam("repeat", rep_space);

    // std::vector<std::string> algos = {"MaierClo", "AFPClo", "MinCoverClo", "DuquenneClo", "BercziClo"}; //, "MinCover", "Berczi"};
    std::vector<std::string> algos = {"MinCoverClo", "DuquenneClo", "MaierClo", "BercziClo", "AFPClo"};

    boost::timer::auto_cpu_timer t;
    tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos, root + filename);


//    root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/Finetune/Maier/";
//    filename = "Maier_Red_500impl.csv";
//
//    algos = {"MaierClo", "MaierLin"};
//    tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos, root + filename);

    return 0;
}


//int main(int, char **){
//    GridTester tester;
//    std::vector<std::string> algos = {"DuquenneLin", "MaierLin"};
//    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/Real/";
//    std::string filename = root + "DuqMaierLin";
//    std::map<std::string, std::string> names;
//
//    std::string imp_root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/RealDatasets/";
//    std::vector<std::string> testname = {"SPECT", "flare", "post-operative", "vote", "zoo", "breast-cancer", "breast-w"};
//    //std::vector<std::string> testname = {"SPECT", "flare", "breast-cancer", "breast-w"};
//
//    std::vector<std::string> basistype = {"min", "DG", "proper", "min2", "mingen"};
//    std::string ithname;
//
//    for (auto &n : testname){
//        for(auto &t : basistype){
//            ithname = n;
//            ithname += "_" + t;
//            names[ithname] = imp_root + ithname + ".imp";
//        }
//    }
//
//    tester.NamedTest(names, algos, filename);
//    return 0;
//
//}

/// template main
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
