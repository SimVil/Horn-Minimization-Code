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
#include "../../CanonicalBasis/horn_minimization_algo.h"
#include <boost/timer/timer.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/sum.hpp>


typedef std::pair<unsigned, unsigned> bounds;
typedef std::vector<FCA::ImplicationInd> theory;

typedef boost::accumulators::stats<boost::accumulators::tag::mean,
        boost::accumulators::tag::min,
        boost::accumulators::tag::max,
        boost::accumulators::tag::sum,
        boost::accumulators::tag::moment<2>> metrics;

typedef boost::accumulators::accumulator_set<double, metrics> statistics;

typedef struct result result_t;

struct result {
    unsigned implNum, attrNum;
    unsigned gen, repeat;
    double mean, min_t, max_t, var_t;

    result(std::map<std::string, unsigned> &tparam, statistics &tres);
    result() = default;

};

void displayTestCase(std::vector<std::string> names, unsigned implNum, unsigned attrNum, unsigned gen);



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
    std::map<std::string, void(*)(const theory &, theory &)> algos;

    void GridSearch(const std::vector<std::string> &param, std::vector<std::vector<unsigned>> &nuplets);
    void ExportResults(std::string &filename, std::list<std::map<std::string, result_t>> results);


public:
    GridTester(std::vector<std::string>, const bounds &b, int (*)(int));
    GridTester(std::vector<std::string>, const bounds &b);
    GridTester();

    void PerformTestCase(std::map<std::string, unsigned> &param,
                         const std::vector<std::string> &algos,
                         std::map<std::string, result_t> &results,
                         bool verbose = false);


    void GridTest(const std::vector<std::string> &param, const std::vector<std::string> &algos, std::string filename);

    void setParam(const std::vector<std::string> &params, const bounds &b);
    void setParam(const std::vector<std::string> &params, const bounds &b, int (*f)(int));
    void setParam(const std::string &param, const bounds &b);
    void setParam(const std::string &param, const bounds &b, int (*f)(int));

    void AvailableAlgos();


};


#endif //ALGORITHMS_GRIDTESTER_H
