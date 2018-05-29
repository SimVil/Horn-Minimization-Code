# include <stdexcept>

# include "fca_linclosure.h"

bool FCA::LinClosure::Apply(const FCA::BitSet &current, const theory &implications,
                            FCA::BitSet &res, const size_t *prefLen, BitSet *implied,  size_t *reach, bool stop)
{
    if (!implications.empty() && (implications.front().Premise().size() != current.size() || implications.front().Conclusion().size() != current.size()))
        throw std::invalid_argument("size of premise and consclusion must agreed with size of current");

    const size_t attrNum = current.size();
    const size_t implNum = implications.size();

    if (implied && implied->size() != implNum)
        throw std::invalid_argument("LinCLosure::Apply - implied must have as much elements as implications");

    BitSet newdep(current);
    std::vector<size_t> count(implNum);
    std::vector<std::vector<size_t> > list(attrNum);

    //initialization

    size_t implInd = 0;
    bool reached = false;

    while(implInd < implNum && !reached){
        count[implInd] = implications[implInd].Premise().count();

        if (count[implInd] == 0)
        {
            newdep |= implications[implInd].Conclusion();
            if (implied)
            {
                if(implied->test(implInd) && stop)
                {
                    *reach = (int) implInd;
                    reached = true;
                }
                implied->set(implInd);
            }
        }

        for (size_t attrInd = 0; attrInd < attrNum; ++attrInd)
            if (implications[implInd].Premise().test(attrInd))
            {
                list[attrInd].push_back(implInd);
            }

        implInd ++;
    }

    std::vector<size_t> update;
    std::vector<bool> use(attrNum, false);

    for (size_t i = 0; i < attrNum; ++i)
        if (newdep.test(i))
        {
            use[i] = true;
            update.push_back(i);					
        }

    //end initialization

    //computation

    while (!update.empty() && !reached)
    {
        size_t ind = update.back();
        update.pop_back();

        for (size_t i = 0; i < list[ind].size(); ++i)			
        {
            size_t impInd = list[ind][i];
            --count[impInd];

            if (count[impInd] == 0)
            {
                BitSet add = implications[impInd].Conclusion() - newdep;
                newdep |= add;

                if(implied)
                {
                    // L |= X --> Premise(impInd)
                    // the next condition deserves to detect direct determination
                    // cf Maier Algorithm
                    if(implied->test(impInd) && stop)
                    {
                        *reach = impInd;
                        reached = true;
                    }
                    implied->set(impInd);
                }


//                if (!IsPrefixIdentical(newdep, current, *prefLen))
//                    return false;

                for (size_t attrInd = 0; attrInd < attrNum; ++attrInd)				
                    if (add.test(attrInd) && !use[attrInd])					
                    {
                        update.push_back(attrInd);											
                        use[attrInd] = true;
                    }
            }
        }		
    }

    //end computation

    res = newdep;
    return true;
}

bool FCA::LinClosure::Apply(const BitSet &current, const std::vector<FCA::ImplicationInd> &implications, std::vector<size_t> &premiseCount,
                            std::vector<std::vector<size_t> > &list, size_t &prevImplSetSize, BitSet &res, const size_t prefLen)
{
    if (!implications.empty() && (implications.front().Premise().size() != current.size() || implications.front().Conclusion().size() != current.size()))
        throw std::invalid_argument("size of premise and consclusion must agreed with size of current");

    const size_t attrNum = current.size();
    const size_t implNum = implications.size();

    //initialization
    
    if (prevImplSetSize != implNum)
    {
        premiseCount.resize(implNum);

        for (size_t implInd = prevImplSetSize; implInd < implNum; ++implInd)
        {			
            premiseCount[implInd] = implications[implInd].Premise().count();		
            for (size_t attrInd = 0; attrInd < attrNum; ++attrInd)			
                if (implications[implInd].Premise().test(attrInd))		
                {					
                    list[attrInd].push_back(implInd);			
                }
        }

        prevImplSetSize = implNum;
    }

    BitSet newdep(current);
    std::vector<size_t> update;	
    std::vector<bool> use(attrNum, false);

    for (size_t i = 0; i < attrNum; ++i)
        if (current.test(i))		
        {
            use[i] = true;
            update.push_back(i);					
        }

    std::vector<size_t> count = premiseCount;

    //end initialization

    //computation

    while (!update.empty())
    {
        size_t ind = update.back();
        update.pop_back();

        for (size_t i = 0; i < list[ind].size(); ++i)			
        {
            size_t impInd = list[ind][i];
            --count[impInd];

            if (count[impInd] == 0)
            {
                BitSet add = implications[impInd].Conclusion() - newdep;
                newdep |= add;

                if (!IsPrefixIdentical(newdep, current, prefLen))
                    return false;

                for (size_t attrInd = 0; attrInd < attrNum; ++attrInd)				
                    if (add.test(attrInd) && !use[attrInd])					
                    {
                        update.push_back(attrInd);											
                        use[attrInd] = true;
                    }
            }
        }		
    }

    //end computation

    res = newdep;
    return true;
}


void FCA::LinClosure::removeImplication(int index, const FCA::ImplicationInd &imp,
                                        std::vector<size_t> &count, std::vector<std::vector<size_t>> &list) {

    count[index] = count.back();
    count.pop_back();

    size_t attrNum = imp.Premise().size();

    for(size_t i = 0; i < attrNum; ++i){
        if (imp.Premise().test(i)){
            list[i][index] = list[i].back();
            list[i].pop_back();
        }

    }
}


void FCA::LinClosure::addImplication(const FCA::ImplicationInd &imp, std::vector<size_t> &count,
                                     std::vector<std::vector<size_t>> &list) {

    count.emplace_back(imp.Premise().count());

    size_t attrNum = imp.Premise().size();
    size_t index = count.size() - 1;

    for(size_t i = 0; i < attrNum; ++i){
        if (imp.Premise().test(i)){
            list[i].emplace_back(index);
        }
    }

}


void FCA::LinClosure::initCounters(const theory &L, std::vector<size_t> &count,
                              std::vector<std::vector<size_t>> &list, const size_t *size) {

    if (count.empty() && list.empty()){
        if (size){
            list.resize(*size);
        } else {
            list.resize(L.front().Premise().size());
        }

        for (auto &imp : L){
            LinClosure::addImplication(imp, count, list);
        }
    }
}


void FCA::LinClosure::Apply(const BitSet &X, const theory &L, BitSet &LX,
                            std::vector<size_t> &count, std::vector<std::vector<size_t> > &list, const size_t *size,
                            BitSet *implied, size_t *reach, bool stop) {

    // if count and list are not empty, does nothing
    LinClosure::initCounters(L, count, list, size);

    std::vector<size_t> ccounter = count;

    BitSet newclosure(X);
    size_t implNum = L.size();
    size_t attrNum = X.size();
    size_t j = 0;

    bool reached = false;

    // deal with non-closed empty set.
    while (j < implNum && !reached){
        if (ccounter[j] == 0) {
            newclosure |= L[j].Conclusion();

            if(implied){
                if(implied->test(j) && stop){
                    *reach = j;
                    reached = true;
                }

                implied->set(j);
            }

        }
        ++j;
    }

    std::vector<size_t> update;
    std::vector<bool> use(attrNum, false);

    for (size_t i = 0; i < attrNum; ++i){
        if (X.test(i)){
            use[i] = true;
            update.push_back(i);
        }
    }

    while(!update.empty() && !reached){
        size_t ind = update.back();
        update.pop_back();

        if(!list[ind].empty()){
            for(size_t i = 0; i < list[ind].size(); ++i)
            {
                size_t impInd = list[ind][i];
                --ccounter[impInd];

                if (ccounter[impInd] == 0)
                {
                    BitSet add = L[impInd].Conclusion() - newclosure;
                    newclosure |= add;

                    if(implied){
                        if(implied->test(impInd) && stop){
                            *reach = impInd;
                            reached = true;
                        }

                        implied->set(impInd);
                    }

                    for (size_t attrInd = 0; attrInd < attrNum; ++attrInd)
                        if (add.test(attrInd) && !use[attrInd])
                        {
                            update.push_back(attrInd);
                            use[attrInd] = true;
                        }
                }
            }
        }

    }

    LX = newclosure;
}
