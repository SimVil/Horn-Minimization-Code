#define CATCH_CONFIG_MAIN

#include <iostream>
#include <vector>
#include "CanonicalBasis/horn_minimization_algo.h"
#include "Tests/Tools/ImplicationTools.h"
#include "Tests/Testers/GridTester.h"
#include <ctime>
#include <boost/timer/timer.hpp>

void LinClosureBenchmark(const std::string &filename){
    size_t impl_max = 2000;
    size_t attr_max = 2000;

    std::ofstream file(filename);

    theory L;
    FCA::BitSet closure, trash;
    double avg, ratio;

    file << "impl,attr,avg,ratio" << std::endl;

    for(size_t i = 100; i < impl_max + 1; i += 100){
        for(size_t j = 100; j < attr_max + 1; j += 100){

            boost::timer::cpu_timer t;
            t.stop();

            for (int k = 0; k < 10; ++k){
                ImplicationTools::GenerateTheory(L, j, i);

                for(int l = 0; l < 10; ++l){
                    closure = ImplicationTools::GetRandomBitSet(j);
                    trash.resize(j);


                    for(int m = 0; m < 10; ++m){
                        t.resume();
                        FCA::LinClosure::Apply(closure, L, trash);
                        t.stop();
                    }

                }
            }

            avg = std::stod(t.format(6, "%t")) / 1000;
            ratio = ((double) i * j) / avg;
            file << i << "," << j << "," << avg << "," << ratio << std::endl;

        }
    }

    file.close();
}


void LinClosureHardBenchmark(const std::string &filename){

    std::ofstream file(filename);

    theory L;
    FCA::BitSet closure, trash;
    FCA::BitSet premise, conclusion;
    double avg, exec;
    int k;

    file << "n,time,avg" << std::endl;

    for(size_t n = 25; n <= 1000; n += 25){
        L.clear();
        L.resize(n - 1);
        conclusion.resize(n);
        premise.resize(n);
        closure.resize(n);
        trash.resize(n);
        conclusion.reset();
        premise.reset();
        closure.reset();
        closure.set(0);

        k = 0;

        for (int i = (int) n - 2; i >= 0; --i){
            premise.set((size_t) i);
            conclusion.set((size_t) i + 1);
            L[k] = FCA::ImplicationInd(premise, conclusion);
            ++k;
            premise.reset();
            conclusion.reset();
        }

        boost::timer::cpu_timer t;
        for(int i = 0; i < 1000; ++i){
            FCA::LinClosure::Apply(closure, L, trash);
        }

        exec = std::stod(t.format(6, "%t"));
        avg = exec / 1000.;

        file << n << "," << exec << "," << avg << std::endl;

    }

    file.close();
}


int main(int, char **){
    srand((unsigned) time(nullptr));
    GridTester tester;
    std::pair<unsigned, unsigned> attr_space(100, 100);
    //std::pair<unsigned, unsigned> impl_space(50, 1000);
    std::vector<unsigned> impl_space = {8500};// 400, 500, 2000, 2300, 2500, 3000, 3500, 3800};
    std::pair<unsigned, unsigned> gen_space(100, 100);
    std::pair<unsigned, unsigned> rep_space(1, 1);

    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/";
    std::string filename = "Triple_MMB_pt5_NRd.csv";

    tester.setParam("implNum", impl_space);
    tester.setParam("attrNum", attr_space, [](int){return 50;});
    tester.setParam("gen", gen_space);
    tester.setParam("repeat", rep_space);

    std::vector<std::string> algos = {"Maier", "MinCover"}; //, "MinCover", "Berczi"};

    boost::timer::auto_cpu_timer t;

    tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos, root + filename);

//    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/BitImplicationFiles/";
//    std::string filename = "impp_";
//
//    theory L, La, Lb;
//    size_t implNum = 10, attrNum = 10;
//    int gen = 500;
//    for(int i = 0; i < gen; ++i){
//        ImplicationTools::GenerateTheory(L, attrNum, implNum, true, true);
//
//        HORN::AFPMinimization(L, La);
//        FCA::MinimalCover(L, Lb);
//
//        if (La.size() != Lb.size()){
//            std::cout << i << " - AFP: " << La.size() << ", " << "Min: " << Lb.size() << std::endl;
//            //ImplicationTools::WriteFile(root + filename + std::to_string(i) + ".imp", L);
//        }
//    }

    return 0;
}

