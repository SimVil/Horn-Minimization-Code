# include "fca_minimization.h"

std::vector<FCA::ImplicationInd> FCA::MinimizeBasis(const std::vector<FCA::ImplicationInd> &implSet)
{
    if (implSet.empty())
    {
        return implSet;
    }

    const size_t implSetSize = implSet.size();
    std::vector<ImplicationInd> tmp = implSet;

    for (size_t implCur = 0; implCur < implSetSize; ++implCur)
    {
        tmp[implCur].Conclusion() |= tmp[implCur].Premise();
    }	

    for (int implCur = static_cast<int>(implSetSize) - 1; implCur >= 0; --implCur)
    {
        ImplicationInd impl = tmp[implCur];		
        tmp[implCur] = tmp.back();
        tmp.pop_back();

        Closure::Apply(impl.Premise(), tmp, impl.Premise());
        if (impl.Premise().is_proper_subset_of(impl.Conclusion()))
        {
            tmp.push_back(impl);
        }
    }

    return tmp;
}


// ==== LinClosure template instanciation ====
template<>
void FCA::MinimalCover<FCA::LinClosure>(const theory &L, theory &Lc)
{
    Lc.clear();
    if (L.empty()){ return; }

    for (auto imp : L){
        imp.Conclusion() |= imp.Premise();
        Lc.emplace_back(imp);
    }

    std::vector<size_t> count;
    std::vector<std::vector<size_t>> list;

    for(auto &imp : Lc){
        FCA::LinClosure::Apply(imp.Conclusion(), Lc, imp.Conclusion(), count, list);
    }

    size_t size = Lc.size() - 1;
    FCA::ImplicationInd imp;

    for(auto i = (int) size; i >= 0; --i){
        imp = Lc[i];
        Lc[i] = Lc.back();
        //FCA::LinClosure::removeImplication(i, imp, count, list);
        Lc.pop_back();

        FCA::Closure::Apply(imp.Premise(), Lc, imp.Premise());

        if (imp.Premise().is_proper_subset_of(imp.Conclusion())) {
            Lc.push_back(imp);
            //FCA::LinClosure::addImplication(imp, count, list);
        }

    }

}



/* Old version of MinCover */

/*
const size_t attrNum = L.front().Premise().size();
const size_t implSetSize = L.size();
Lc = L;

for (size_t implCur = 0; implCur < implSetSize; ++implCur)
{
    Lc[implCur].Conclusion() |= Lc[implCur].Premise();
}

std::vector<BitSet> list(attrNum);
size_t prevImplSetSize = 0;

for (size_t implCur = 0; implCur < implSetSize; ++implCur)
{
    ImplicationInd impl = Lc[implCur];

    //LinClosureImproved::Apply(impl.Conclusion(), Lc, list, prevImplSetSize, impl.Conclusion());
    LinClosure::Apply(impl.Conclusion(), Lc, impl.Conclusion());
    Lc[implCur] = impl;
}

for (int implCur = static_cast<int>(implSetSize) - 1; implCur >= 0; --implCur)
{
    ImplicationInd impl = Lc[implCur];
    Lc[implCur] = Lc.back();
    Lc.pop_back();

    LinClosure::Apply(impl.Premise(), Lc, impl.Premise());
    if (impl.Premise().is_proper_subset_of(impl.Conclusion()))
    {
        Lc.push_back(impl);
    }
}
*/