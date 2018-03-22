//
// Created by Simon on 22/03/2018.
//

#ifndef ALGORITHMS_GRIDTESTER_H
#define ALGORITHMS_GRIDTESTER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cassert>
#include "../Tools/ImplicationTools.h"
#include "../../CanonicalBasis/fca_implications.h"
#include <boost/timer/timer.hpp>


typedef std::pair<unsigned, unsigned> bounds;

class interval {
protected:
    int (*range_function)(int);
    unsigned inf, sup;

public:

    interval(unsigned i, unsigned s, int (*f)(int));
    interval(unsigned i, unsigned s);
    interval() = default;

    void setInterval(unsigned i, unsigned j) { assert(j >= i); inf = i, sup = j ;};
    void setRangefunction(int (*f)(int)) {range_function = f;};
    void getRange( std::vector<unsigned> &range);
};

class GridTester {
protected:
    std::map<std::string, interval> parameters;
    unsigned systems_per_case;

public:
    GridTester(std::vector<std::string>, const bounds &b, int (*)(int));
    GridTester(std::vector<std::string>, const bounds &b);

    void GridSearch(const std::vector<std::string> &param);

    void setParam(const std::vector<std::string> &params, const bounds &b);
    void setParam(const std::vector<std::string> &params, const bounds &b, int (*f)(int));
    void setParam(const std::string &param, const bounds &b);
    void setParam(const std::string &param, const bounds &b, int (*f)(int));


};


#endif //ALGORITHMS_GRIDTESTER_H
