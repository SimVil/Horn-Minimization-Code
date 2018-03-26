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
//    GridTester tester;
//    std::pair<unsigned, unsigned> attr_space(1000, 2000);
//    std::pair<unsigned, unsigned> impl_space(50, 150);
//    std::pair<unsigned, unsigned> gen_space(20, 20);
//    std::pair<unsigned, unsigned> rep_space(5, 5);
//
    std::string root = "D:/Documents/Courses/Master Thesis/Code/Algorithms/Tests/CSVLogs/";
    std::string filename = "LinClosure.csv";
    std::vector<std::string> vec = {"Lc_HE_1.csv", "Lc_HE_2.csv", "Lc_HE_3.csv",
                                    "Lc_HE_4.csv", "Lc_HE_5.csv", "Lc_HE_6.csv",
                                    "Lc_HE_7.csv", "Lc_HE_8.csv", "Lc_HE_9.csv"};
//
//    tester.setParam("implNum", impl_space, [](int){return 10;});
//    tester.setParam("attrNum", attr_space, [](int){return 100;});
//    tester.setParam("gen", gen_space);
//    tester.setParam("repeat", rep_space);
//
//    std::vector<std::string> algos = {"Maier", "MinCover"};
//    tester.GridTest({"implNum", "attrNum", "gen", "repeat"}, algos, root + filename);

    LinClosureBenchmark(root + filename);


    return 0;
}

