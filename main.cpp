#include <iostream>
#include "CanonicalBasis/horn_maier.h"
#include "CanonicalBasis/horn_berczi.h"
#include "CanonicalBasis/test_functions.h"
#include <boost/timer/timer.hpp>
//#include <gperftools/profiler.h>


int main(int, char **) {

    size_t implNum = 100;
    size_t attrNum = 50;
    int nummin = 50;

    std::vector<FCA::ImplicationInd> L(implNum);
    std::vector<FCA::ImplicationInd> maierL;
    std::vector<FCA::ImplicationInd> bercziL;

    for(size_t i = 0; i < implNum; ++i)
    {
        L[i] = FCA::ImplicationInd(GetRandomBitSet(attrNum, 0.75), GetRandomBitSet(attrNum, 0.50));
    }

    boost::timer::auto_cpu_timer t;
    //ProfilerStart("/home/simon/share/Algorithms/out.prof");
    for(int j = 0; j < nummin; ++j)
    {
        maierL = HORN::MaierMinimization(L);
        bercziL = HORN::BercziMinimization(L);
    }
    //ProfilerStop();
    return 0;
}