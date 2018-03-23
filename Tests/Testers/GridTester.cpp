//
// Created by Simon on 22/03/2018.
//

#include <horn_shock.h>
#include "GridTester.h"

int identity(int x) { return x; }

interval::interval(const unsigned i, const unsigned s, int (*f)(int)) {
    inf = i;
    sup = s;
    range_function = f;
}

interval::interval(const unsigned i, const unsigned s) {
    inf = i;
    sup = s;
    range_function = identity;
}

void interval::getRange(std::vector<unsigned> &range) {
    range.clear();
    unsigned value = inf;
    range.emplace_back(inf);

    while ((unsigned) range_function(value) < sup){
        value++;
        range.emplace_back((unsigned) range_function(value));
    }

    if (range.back() != sup){ range.emplace_back(sup); }

}


GridTester::GridTester(const std::vector<std::string> keys, const std::pair<unsigned, unsigned> &bounds, int (*f)(int))
: parameters(std::map<std::string, interval>()){
    for (auto &s : keys){
        interval t(bounds.first, bounds.second, f);
        parameters.insert(std::make_pair(s, t));
    }

}


GridTester::GridTester(std::vector<std::string> keys, const std::pair<unsigned, unsigned> &bounds)
: parameters(std::map<std::string, interval>()) {
    for (auto &s : keys){
        interval t(bounds.first, bounds.second);
        parameters.insert(std::make_pair(s, t));
    }
}


GridTester::GridTester() :
        parameters(std::map<std::string, interval> ()),
        algos(std::map<std::string, void (*)(const theory &, theory &)> ()) {

    algos["Maier"] = HORN::MaierMinimization;
    algos["Shock"] = HORN::ShockMinimization;
    algos["Berczi"] = HORN::BercziMinimization;
    algos["MinCover"] = FCA::MinimalCover;


};

void GridTester::GridSearch(const std::vector<std::string> &param, std::vector<std::vector<unsigned>> &nuplets) {

    std::vector<unsigned> sizes;
    std::vector<unsigned> enumeration;
    std::vector<unsigned> uplet;
    std::vector<std::vector<unsigned>> ranges;
    nuplets.clear();


    unsigned int product_size = 1;

    for (const std::string &s : param){

        ranges.emplace_back(std::vector<unsigned> ());
        parameters[s].getRange(ranges.back());
        sizes.emplace_back(ranges.back().size());
        enumeration.emplace_back(0);
        product_size = product_size * sizes.back();
    }

    unsigned int i = 0;
    nuplets.resize(product_size);
    uplet.resize(enumeration.size());

    while (i < product_size){
        std::fill(uplet.begin(), uplet.end(), 0);

        for (size_t j = 0; j < enumeration.size(); ++j){
            uplet[j] = ranges[j][enumeration[j]];
            std::cout << uplet[j] << " ";
        }

        nuplets[i] = uplet;
        std::cout << std::endl;

        for (int j = (int) enumeration.size() - 1; j >= 0; --j){
            if(j == enumeration.size() - 1){
                enumeration[j] = (enumeration[j] + 1);
            } else {
                if (enumeration[j + 1] == sizes[j + 1]){
                    enumeration[j + 1] = 0;
                    enumeration[j] = (enumeration[j] + 1);
                }
            }

        }

        ++i;
    }

}

void GridTester::setParam(const std::vector<std::string> &params, const std::pair<unsigned, unsigned> &bounds) {
    for (const std::string &s : params){
        setParam(s, bounds);
    }
}

void GridTester::setParam(const std::vector<std::string> &params, const std::pair<unsigned, unsigned> &bounds,
                          int (*f)(int)) {
    for (const std::string &s : params){
        setParam(s, bounds, f);
    }
}

void GridTester::setParam(const std::string &param, const std::pair<unsigned, unsigned> &bounds) {
    setParam(param, bounds, identity);
}

void GridTester::setParam(const std::string &param, const std::pair<unsigned, unsigned> &bounds, int (*f)(int)) {
    parameters[param].setInterval(bounds.first, bounds.second);
    parameters[param].setRangefunction(f);

}


void GridTester::PerformTestCase(std::map<std::string, unsigned> &param,
                                 std::vector<std::string> &algnm,
                                 std::map<std::string, double> &results) {

    theory L, Lc;

    std::map<std::string, boost::accumulators::accumulator_set<double,
            boost::accumulators::stats<boost::accumulators::tag::mean,
                    boost::accumulators::tag::min,
                    boost::accumulators::tag::max,
                    boost::accumulators::tag::sum,
                    boost::accumulators::tag::moment<2>>>> acc;

    for(unsigned i = 0; i < param["gen"]; ++i){
        ImplicationTools::GenerateTheory(L, param["attrNum"], param["implNum"]);
        //boost::timer::auto_cpu_timer gt;

        std::cout << i << std::endl;
        for (auto &s: algnm){
            if (algos.find(s) != algos.end()){
                boost::timer::cpu_timer t;
                for (unsigned j = 0; j < param["repeat"]; ++j){
                    algos[s](L, Lc);

                    t.stop();
                    Lc.clear();
                    t.resume();
                }

                acc[s](std::stod(t.format(5, "%t")) / (double) param["repeat"]);
            }


        }

    }

    for (auto &p : acc){
        std::cout << "TestCase: " << p.first << " (" << param["attrNum"] << ", " << param["implNum"] << ")";
        std::cout << std::endl << "---------------------------" << std::endl << std::endl;
        std::cout << "Mean: " << boost::accumulators::mean(acc[p.first]) << std::endl;
        std::cout << "Min/Max: " << boost::accumulators::min(acc[p.first]) << " ";
        std::cout <<  boost::accumulators::max(acc[p.first]) << std::endl;
        std::cout << "Variance: " << boost::accumulators::moment<2>(acc[p.first]) << std::endl;
        std::cout << "Sum of average times: " << boost::accumulators::sum(acc[p.first]) << std::endl;
        std::cout << "----------------" << std::endl;
    }


}

void GridTester::AvailableAlgos() {
    std::cout << "Available algorithms:" << std::endl << std::endl;
    for (auto &p : algos){
        std::cout << p.first << " ";
    }

    std::cout << std::endl;

}
