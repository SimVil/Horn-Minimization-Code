#include <iostream>
#include <vector>
#include "CanonicalBasis/test_functions.h"
#include <boost/timer/timer.hpp>

// #include <gperftools/profiler.h>


int main(int, char **) {

    size_t implNum = 4;
    size_t attrNum = 5;

    srand(time(NULL));

    std::vector<std::string> sigma = {"a", "b", "c", "d", "e"};
    std::vector<FCA::ImplicationInd> L(implNum);
    std::vector<FCA::ImplicationInd> Lbis;

    for(size_t i = 0; i < implNum; ++i)
    {
        L[i] = FCA::ImplicationInd(GetRandomBitSet(attrNum, 0.50), GetRandomBitSet(attrNum, 0.50));
    }

    FCA::BitSet pouet = GetRandomBitSet(100, 0.5);
    boost::timer::auto_cpu_timer t;

    for(unsigned long j = 0; j < 10000000; ++j)
    {
        pouet.test(0);
    }

    //std::cout << L.front().Premise().to_ulong() << std::endl;
    //PrintImplications(std::cout, FCA::Convert(L, sigma));
  
    return 0;
}