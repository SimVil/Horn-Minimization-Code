//
// Created by Simon on 22/03/2018.
//

#include <horn_shock.h>
#include "GridTester.h"

int identity(int x) { return x; }

void
displayTestCase(std::vector<std::string> names, unsigned implNum, unsigned attrNum, unsigned gen) {
    std::cout << "Algos tested: ";
    for (auto &s : names){
        std::cout << s << " ";
    }
    std::cout << std::endl;
    std::cout << "sizes(M, L, Gen): (" << attrNum << ", " << implNum << ", " << gen << ")" << std::endl;
    std::cout << "Progress: ";
}

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
    unsigned x = 0;
    range.emplace_back(inf);

    while ((unsigned) value + range_function(x) < sup){
        x++;
        value += (unsigned) range_function(x);
        range.emplace_back((unsigned) value);
    }

    if (range.back() != sup){ range.emplace_back(sup); }

}


result::result(std::map<std::string, unsigned> &tparam, statistics &tres) :
        implNum(tparam["implNum"]), attrNum(tparam["attrNum"]), gen(tparam["gen"]), repeat(tparam["repeat"]),
        mean(1.), min_t(0.), max_t(0.), var_t(0.1) {
    mean = (double) boost::accumulators::extract_result<boost::accumulators::tag::mean>(tres);
    min_t = (double) boost::accumulators::extract_result<boost::accumulators::tag::min>(tres);
    max_t = (double) boost::accumulators::extract_result<boost::accumulators::tag::max>(tres);
    var_t = (double) boost::accumulators::extract_result<boost::accumulators::tag::moment<2>>(tres);
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

    std::cout << "#======= Generating test cases: ";
    double tenth = 0.1 * (double) product_size;
    double percent = tenth;

    while (i < product_size){
        std::fill(uplet.begin(), uplet.end(), 0);

        percent = ((double) i / (double) product_size);
        if (percent >= tenth){
            tenth += tenth;
            std::cout << "==";
        }

        for (size_t j = 0; j < enumeration.size(); ++j){
            uplet[j] = ranges[j][enumeration[j]];
        }

        nuplets[i] = uplet;

        for (int j = (int) enumeration.size() - 1; j >= 0; --j){
            if(j == (int) enumeration.size() - 1){
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

    std::cout << " Done!" << std::endl << std::endl;

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
                                 const std::vector<std::string> &algnm,
                                 std::map<std::string, result_t> &results, bool verbose){

    theory L, Lc;
    FCA::BitSet emptyset(param["attrNum"]);
    emptyset.reset();

    std::map<std::string, statistics> acc;

    if (verbose)
        displayTestCase(algnm, param["implNum"], param["attrNum"], param["gen"]);

    for(unsigned i = 0; i < param["gen"]; ++i){
        ImplicationTools::GenerateTheory(L, param["attrNum"], param["implNum"]);

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

        if (verbose){
            double percent = ((double) i / (double) param["gen"]) * 100.;
            if (((int) percent) % 10 == 0){
                std::cout << "==";
            }

            if ((int) percent >= 90){
                std::cout << " Done! " << std::endl << std::endl;
            }
        }
    }

    for (auto &p : acc){
        results[p.first] = result_t(param, p.second);
    }


}

void GridTester::AvailableAlgos() {
    std::cout << "Available algorithms:" << std::endl << std::endl;
    for (auto &p : algos){
        std::cout << p.first << " ";
    }

    std::cout << std::endl;

}


void GridTester::GridTest(const std::vector<std::string> &param, const std::vector<std::string> &algs,
                          std::string filename){

    // vector dedicated to implications testing
    std::vector<std::string> thp = {"implNum", "attrNum", "gen", "repeat"};

    for (auto &s: param){
        assert(parameters.find(s) != parameters.end());

        // implication theory specific verification
        assert(std::find(thp.begin(), thp.end(), s) != thp.end());
    }

    for (auto &s : algs){
        assert(algos.find(s) != algos.end());
    }

    // 1: generate test cases tuples
    std::vector<std::vector<unsigned>> nuplets;
    GridSearch(param, nuplets);

    std::list<std::map<std::string, result_t>> results;
    std::map<std::string, result_t> testcase;
    std::map<std::string, unsigned> testcaseparam;

    for (auto &s : algs){
        testcase[s] = result_t();
    }

    int i = 0, j = 0;
    int size = nuplets.size();
    double tenth = 0.1 * (double) size;
    double percent = tenth;

    std::cout << "##====== Performing test cases: ";

    for (auto &uplet : nuplets) {

        i = 0;
        for (auto &s: param){
            testcaseparam[s] = uplet[i];
            ++i; // thanks to gridsearch, i should not exceed size(uplet)
        }

        PerformTestCase(testcaseparam, algs, testcase);

        percent = ((double) j / (double) size);
        if (percent >= tenth){
            tenth += tenth;
            std::cout << " " << percent * 100. << "% ";

        }
        j++;

        results.emplace_back(testcase);
    }

    std::cout << " Done!" << std::endl << std::endl;

    ExportResults(filename, results);

}


void GridTester::ExportResults(std::string &filename, std::list<std::map<std::string, result_t>> results) {
    std::ofstream file(filename);

    if (file.is_open()){

        std::cout << "###===== Exporting to CSV file: ";

        file << "Algo," << "attrNum," << "implNum," << "gen," << "repeat,";
        file << "min," << "max," << "mean," << "var" << std::endl;

        int size = results.size();
        int j = 0;

        for (auto &testcase : results){

            for (auto &algo: testcase){
                file << algo.first << ",";
                file << algo.second.attrNum << "," << algo.second.implNum << "," << algo.second.gen  << ",";
                file << algo.second.repeat << "," << algo.second.min_t << "," << algo.second.max_t << ",";
                file << algo.second.mean << "," << algo.second.var_t << std::endl;
            }

            double percent = ((double) j / (double) size) * 100.;
            if ((int) percent % 10 == 0){
                std::cout << "==";

            }
            j++;
        }

        file.close();
        std::cout << " Done!" << std::endl << std::endl;

    } else {
        std::cout << "GridTester::ExportResults - unable to open file: " << filename << std::endl;
    }

}
