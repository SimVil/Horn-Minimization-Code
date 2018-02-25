#include <iostream>
#include <vector>
#include "CanonicalBasis/test_functions.h"
#include "CanonicalBasis/graph_FDgraph.h"
#include <time.h>
// #include <boost/timer/timer.hpp>

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

    PrintImplications(std::cout, FCA::Convert(L, sigma));
    GRAPH::FDGraph g(L);
    Lbis = g.Convert();

    PrintImplications(std::cout, FCA::Convert(Lbis, sigma));
  
    return 0;
}