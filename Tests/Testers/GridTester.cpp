//
// Created by Simon on 22/03/2018.
//

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


void GridTester::GridSearch(const std::vector<std::string> &param) {

    std::vector<unsigned> sizes;
    std::vector<unsigned> enumeration;
    std::vector<unsigned> uplet;
    std::vector<std::vector<unsigned>> ranges;
    std::vector<std::vector<unsigned>> nuplets;

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

        for (int j = 0; j < enumeration.size(); ++j){
            uplet[j] = ranges[j][enumeration[j]];
            std::cout << uplet[j] << " ";
        }

        nuplets[i] = uplet;
        std::cout << std::endl;

        for (int j = enumeration.size() - 1; j >= 0; --j){
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
