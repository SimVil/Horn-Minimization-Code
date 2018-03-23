//
// Created by Simon on 22/03/2018.
//

#include "ImplicationTools.h"

#ifndef MAX_ITER
#define MAX_ITER 500
#endif


void ImplicationTools::ReadImplication(const std::string &line, const size_t attrNum, theory &L) {

    size_t implsize = 2;  // premise (1) + conclusion (1) = 2
    std::vector<std::string> implication(implsize);
    std::vector<std::vector<std::string>> sets(implsize);  // indices of premise and conclusion
    std::vector<FCA::BitSet> bitimpl(implsize);
    size_t index;

    boost::split(implication, line, boost::is_any_of(">"));
    for(size_t i = 0; i < implsize; ++i){
        boost::trim_if(implication[i], boost::is_any_of(" \r"));
        boost::split(sets[i], implication[i], boost::is_any_of(" "));
        bitimpl[i].resize(attrNum);
        bitimpl[i].reset();

        for(std::string &s : sets[i]){
            if (!s.empty()){
                index = (size_t) std::stoi(s);
                assert(index >= 0 && index < attrNum);
                bitimpl[i].set(index);
            }
        }
    }

    assert(!bitimpl[1].none());
    L.emplace_back(FCA::ImplicationInd(bitimpl[0], bitimpl[1]));

}


void ImplicationTools::ReadFile(std::string filename, theory &L) {

    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> metadata(2);
    size_t implNum, attrNum, i;

    // given theory is replaced
    L.clear();


    if (file.is_open()){
        std::getline(file, line);
        boost::trim(line);
        boost::split(metadata, line, boost::is_any_of(" "));

        assert(metadata.size() == 2);
        attrNum = (size_t) std::stoi(metadata[0]);
        implNum = (size_t) std::stoi(metadata[1]);

        i = 0;

        while(std::getline(file, line) && i < implNum){
            ImplicationTools::ReadImplication(line, attrNum, L);
            i = L.size();
        }

        file.close();

    } else {
        std::cout << "ImplicationTools::ReadFile: unable to open file " + filename << std::endl;
    }

}

void ImplicationTools::WriteFile(std::string filename, const theory &L) {

    if (L.empty()) {
        std::cout << "Empty theory, no file written" << std::endl;
        return;
    }

    size_t attrNum = L.front().Premise().size();
    size_t implNum = L.size();

    std::ofstream file(filename);

    if (file.is_open()){
        file << attrNum << " " << implNum << std::endl;
        for (auto &impl : L){
            for(size_t i = 0; i < attrNum; ++i){
                if (impl.Premise().test(i)){
                    file << i << " ";
                }
            }

            file << "> ";

            for(size_t i = 0; i < attrNum; ++i){
                if (impl.Conclusion().test(i)){
                    file << i << " ";
                }
            }

            file << std::endl;
        }

    } else {
        std::cout << "ImplicationTools::WriteFile: unable to open file or non-empty file " + filename << std::endl;
    }

    file.close();

}

void ImplicationTools::ExpandTheory(theory &L, double growth) {

    if (L.empty() || growth < 0. || growth > 1.)
        return;

    size_t growth_size = L.size() + (int) (L.size() * growth);

    int inda, indb;
    FCA::ImplicationInd impl;

    std::vector<FCA::ImplicationInd (*)(const FCA::ImplicationInd&, const FCA::ImplicationInd&)> functions;
    functions.emplace_back(ImplicationTools::ArmstrongPTransitivity);
    functions.emplace_back(ImplicationTools::ArmstrongUnion);
    functions.emplace_back(ImplicationTools::ArmstrongAugmentationP);
    functions.emplace_back(ImplicationTools::ArmstrongAugmentationC);
    functions.emplace_back(ImplicationTools::ArmstrongReductionC);
    functions.emplace_back(ImplicationTools::ArmstrongReductionP);

    boost::random::mt19937 gen(rand());
    boost::random::uniform_int_distribution<int> implchoice(0, growth_size - 1);
    boost::random::uniform_int_distribution<int> fchoice(0, functions.size() - 1);


    while (L.size() < growth_size){
        inda = implchoice(gen) % L.size();
        indb = implchoice(gen) % L.size();

        impl = functions[fchoice(gen)](L[inda], L[indb]);
        L.emplace_back(impl);

        ImplicationTools::Reduce(L);

    }

}

void ImplicationTools::GenerateTheory(theory &L, size_t attrNum, size_t implNum) {

    L.clear();
    FCA::ImplicationInd tmp;
    int emptypremise = 0;
    size_t iter;

    for(size_t i = 0; i < implNum; ++i){
        iter = 0;// if we fail to generate satisfying implications, we accept the next one.
        do {
            tmp = ImplicationTools::GetRandomImplication(attrNum);
            ++iter;

            // allow for only one implication with null premise
            if (emptypremise <= 1) {
                emptypremise += (int) tmp.Premise().none();
            }

        } while (emptypremise > 1 && tmp.Premise().none() && iter < MAX_ITER);
        L.emplace_back(tmp);
    }
}


FCA::ImplicationInd ImplicationTools::ArmstrongUnion(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B) {
    FCA::ImplicationInd nimp = A;
    nimp.Premise() |= B.Premise();
    nimp.Conclusion() |= B.Conclusion();
    return nimp;
}

FCA::ImplicationInd
ImplicationTools::ArmstrongPTransitivity(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B) {
    FCA::ImplicationInd nimp;
    FCA::BitSet premise = A.Premise();
    premise |= (B.Premise() - A.Conclusion());
    nimp.Premise() = premise;
    nimp.Conclusion() = B.Conclusion();

    return nimp;
}

FCA::BitSet ImplicationTools::GetRandomBitSet(size_t attrNum) {
    FCA::BitSet set(attrNum);

    boost::random::mt19937 gen(rand());
    boost::random::uniform_int_distribution<int> distribution(0, attrNum - 1);

    int number = distribution(gen);
    for(int i = 0; i < number; ++i){
        set.set((size_t) distribution(gen));
    }

    return set;
}


FCA::ImplicationInd ImplicationTools::GetRandomImplication(size_t attrNum) {
    FCA::BitSet premise(attrNum), conclusion(attrNum);
    do{
        conclusion = ImplicationTools::GetRandomBitSet(attrNum);
        premise = ImplicationTools::GetRandomBitSet(attrNum);
        premise -= conclusion;
    } while (conclusion.none());

    return FCA::ImplicationInd(premise, conclusion);
}


void ImplicationTools::Reduce(theory &L) {
    if (L.empty()) { return ; }

    size_t size = L.size();
    size_t attrNum = L.front().Premise().size();
    FCA::BitSet premise(attrNum), conclusion(attrNum);
    std::map<FCA::BitSet, std::list<FCA::BitSet>> l;

    for (size_t i = 0; i < size; ++i){
        l[L[i].Premise()].emplace_back(L[i].Conclusion());
    }

    L.clear();

    for (auto &p : l){
        premise.reset();
        conclusion.reset();

        premise = p.first;
        for(auto &c : p.second){
            conclusion |= c;
        }

        L.emplace_back(FCA::ImplicationInd(premise, conclusion));
    }
}

void ImplicationTools::CloseSet(const FCA::BitSet &X, theory &L) {
    auto it = L.begin();
    bool stop;

    do {
        auto tmp = it;
        ++it;
        stop = it == L.end();

        if (tmp->Premise().is_subset_of(X) && !tmp->Conclusion().is_subset_of(X)){
            L.erase(tmp);
        }

    } while (!stop);

}

FCA::ImplicationInd
ImplicationTools::ArmstrongAugmentationP(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B) {
    FCA::ImplicationInd impl = A;
    impl.Premise() |= B.Premise();

    return impl;
}

FCA::ImplicationInd
ImplicationTools::ArmstrongAugmentationC(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B) {
    FCA::ImplicationInd impl = A;
    impl.Premise() |= B.Conclusion();

    return impl;
}

FCA::ImplicationInd ImplicationTools::ArmstrongReductionC(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B) {
    FCA::ImplicationInd impl = A;
    impl.Conclusion() &= B.Conclusion();

    return impl;
}

FCA::ImplicationInd ImplicationTools::ArmstrongReductionP(const FCA::ImplicationInd &A, const FCA::ImplicationInd &B) {
    FCA::ImplicationInd impl = A;
    impl.Conclusion() &= B.Premise();

    return impl;
}
