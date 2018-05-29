//
// Created by Simon on 22/03/2018.
//

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
    range = std::vector<unsigned>();
}

interval::interval(const unsigned i, const unsigned s) {
    inf = i;
    sup = s;
    range_function = identity;
    range = std::vector<unsigned>();
}

void interval::getRange(std::vector<unsigned> &u_range) {
    u_range.clear();

    if(range.empty()){
        unsigned value = inf;
        unsigned x = 0;
        range.emplace_back(inf);

        while (value + range_function(x) < sup){
            x++;
            value += (unsigned) range_function(x);
            range.emplace_back(value);
        }

        if (range.back() < sup){ range.emplace_back(sup); }
    }

    u_range = range;
}

void interval::setRange(const std::vector<unsigned> &udef_range) {
    range = udef_range;
}


result::result(std::map<std::string, unsigned> &tparam, statistics &tres) :
        implNum(tparam["implNum"]), attrNum(tparam["attrNum"]), gen(tparam["gen"]), repeat(tparam["repeat"]),
        mean(1.), min_t(0.), max_t(0.), var_t(0.1) {
    mean = (double) boost::accumulators::extract_result<boost::accumulators::tag::mean>(tres);
    min_t = (double) boost::accumulators::extract_result<boost::accumulators::tag::min>(tres);
    max_t = (double) boost::accumulators::extract_result<boost::accumulators::tag::max>(tres);
    var_t = (double) boost::accumulators::extract_result<boost::accumulators::tag::variance>(tres);
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

    algos["MaierClo"] = HORN::MaierMinimization<FCA::Closure, FCA::Closure>;
    algos["MaierLin"] = HORN::MaierMinimization<FCA::Closure, FCA::LinClosure>;
    algos["BercziLin"] = HORN::BercziMinimization<FCA::LinClosure>;
    algos["BercziClo"] = HORN::BercziMinimization<FCA::Closure>;
    algos["MinCoverLin"] = FCA::MinimalCover<FCA::LinClosure>;
    algos["MinCoverClo"] = FCA::MinimalCover<FCA::Closure>;
    algos["AFPLin"] = HORN::AFPMinimization<FCA::LinClosure>;
    algos["AFPClo"] = HORN::AFPMinimization<FCA::Closure>;
    algos["DuquenneLin"] = HORN::DuquenneMinimization<FCA::LinClosure>;
    algos["DuquenneClo"] = HORN::DuquenneMinimization<FCA::Closure>;


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

    while (i < product_size){
        std::fill(uplet.begin(), uplet.end(), 0);

        if (i >= tenth){
            tenth += 0.1 * (double) product_size;;
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

void GridTester::setParam(const std::vector<std::string> &params, const std::vector<unsigned> &udef_r) {
    for (const std::string &s : params){
        setParam(s, udef_r);
    }
}

void GridTester::setParam(const std::string &param, const std::vector<unsigned> &udef_r) {
    parameters[param].setRange(udef_r);
}


void GridTester::PerformTestCase(std::map<std::string, unsigned> &param,
                                 const std::vector<std::string> &algnm,
                                 std::map<std::string, result_t> &results, bool verbose){

    theory L, Lc;
    FCA::BitSet emptyset(param["attrNum"]);
    emptyset.reset();
    int buf = 0;
    int percent = 0;
    bool comput = false;

    std::map<std::string, statistics> acc;
    boost::random::mt19937 gen(rand());
    boost::random::uniform_int_distribution<int> implchoice(1, 9);

    if (verbose)
        displayTestCase(algnm, param["implNum"], param["attrNum"], param["gen"]);

    for(unsigned i = 0; i < param["gen"]; ++i){
        percent = implchoice(gen);
        ImplicationTools::GenerateTheory(L, param["attrNum"], param["implNum"]);
//        ImplicationTools::GenerateTheory(L, param["attrNum"], (param["implNum"] * percent) / 10);
//        double growth = (10./ ((double) percent)) - 1.0;
//        ImplicationTools::ExpandTheory(L, growth);

        comput = false;
        for (auto &s: algnm){

            if (algos.find(s) != algos.end()){
                boost::timer::cpu_timer t;
                for (unsigned j = 0; j < param["repeat"]; ++j){

                    algos[s](L, Lc);

                    t.stop();
//                    if(!comput){
//                        std::cout << Lc.size() << std::endl;
//                        comput = true;
//                    }

                    Lc.clear();
                    t.resume();
                }
                acc[s](std::stod(t.format(8, "%t")) / (double) param["repeat"]);
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

    // results is one mapping per element of the parameter product
    // space
    std::list<std::map<std::string, result_t>> results;
    std::map<std::string, result_t> testcase;
    std::map<std::string, unsigned> testcaseparam;

    for (auto &s : algs){
        testcase[s] = result_t();
    }

    int i = 0, j = 0;
    int size = (int) nuplets.size();
    double tenth = 0.1 * (double) size;
    int percent = 10;

    std::cout << "##====== Performing test cases: ";

    for (auto &uplet : nuplets) {

        i = 0;
        for (auto &s: param){
            testcaseparam[s] = uplet[i];
            ++i; // thanks to gridsearch, i should not exceed size(uplet)
        }

        PerformTestCase(testcaseparam, algs, testcase);

        if ((double)j >= tenth){
            tenth += 0.1 * (double) size;
            std::cout << " " << percent << "% ";
            percent += 10;

        }
        j++;

        results.emplace_back(testcase);
        ExportResults(filename, results);
    }

    std::cout << " Done!" << std::endl << std::endl;



}


void GridTester::ExportResults(std::string &filename, std::list<std::map<std::string, result_t>> results) {
    std::ofstream file(filename);

    if (file.is_open()){

        std::cout << "###===== Exporting to CSV file: ";

        file << "Algo," << "attrNum," << "implNum," << "gen," << "repeat,";
        file << "min," << "max," << "mean," << "var" << std::endl;

        int j = 0;
        double tenth = 0.1 * (double) results.size();

        for (auto &testcase : results){

            for (auto &algo: testcase){
                file << algo.first << ",";
                file << algo.second.attrNum << "," << algo.second.implNum << "," << algo.second.gen  << ",";
                file << algo.second.repeat << "," << algo.second.min_t << "," << algo.second.max_t << ",";
                file << algo.second.mean << "," << algo.second.var_t << std::endl;
            }

            if (j >= tenth){
                tenth += 0.1 * (double) results.size();
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

void GridTester::PerformNamedTest(const std::string name, const std::string &filename,
                                  const std::vector<std::string> &algnm,
                                  std::map<std::string, result_t> &results, bool verbose) {
    theory L, Lc;
    ImplicationTools::ReadFile(filename, L);
    std::map<std::string, statistics> acc;

    std::map<std::string, unsigned> param;
    param["implNum"] = (int) L.size();
    param["attrNum"] = (int) L.front().Premise().size();
    param["repeat"] = 10;
    param["gen"] = 0;


    for(auto &s: algnm){
        boost::timer::cpu_timer t;
        for(int i = 0; i < param["repeat"]; ++i) {
            if (algos.find(s) != algos.end()) {

                algos[s](L, Lc);

                t.stop();
                param["gen"] = (int) Lc.size();
                Lc.clear();
                t.resume();

            }

        }

        acc[s](std::stod(t.format(8, "%t")) / (double) param["repeat"]);

    }

    for (auto &p : acc){
        results[p.first] = result_t(param, p.second);
    }

}


void GridTester::NamedTest(const std::map<std::string, std::string> &names, const std::vector<std::string> &algs,
                           std::string outfile) {

    // one result per test
    std::list<std::map<std::string, result_t>> results;
    std::map<std::string, result_t> testcase;  // results of all algos for one test
    std::string n = outfile;

    for (auto &s : algs){
        testcase[s] = result_t();
    }

    std::cout << "##====== Performing test cases: ";

    auto size = (int) names.size();
    double tenth = 0.1 * (double) size;
    int percent = 10, j = 0;

    for (auto &test : names){
        PerformNamedTest(test.first, test.second, algs, testcase, false);

        if ((double)j >= tenth){
            tenth += 0.1 * (double) size;
            std::cout << " " << percent << "% ";
            percent += 10;

        }
        j++;

        n = outfile + "_" + test.first + ".csv";
        results.emplace_back(testcase);
        ExportResults(n, results);
        results.clear();

    }

    std::cout << " Done!" << std::endl << std::endl;
}


