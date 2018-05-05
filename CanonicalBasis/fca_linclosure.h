# pragma once

# ifndef FCA_LINCLOSURE_H_
# define FCA_LINCLOSURE_H_

# include "fca_datastructures.h"

namespace FCA
{
    class LinClosure
    {	
    public:		
        static bool Apply(const BitSet &current, const std::vector<ImplicationInd> &implications, BitSet &res,
                          const size_t prefLen = 0, BitSet *Implied = NULL, int *reach = NULL);

        static bool Apply(const BitSet &current, const std::vector<ImplicationInd> &implications, std::vector<size_t> &premiseCount,
                          std::vector<std::vector<size_t> > &list, size_t &prevImplSetSize, BitSet &res, const size_t prefLen = 0) ;

        static void Apply(const BitSet &X, const theory &L, BitSet &LX,
                          std::vector<size_t> &count, std::vector<std::vector<size_t> > &list, const size_t *size = nullptr);

        // heavily relies on swapping process
        static void removeImplication(int index, const FCA::ImplicationInd &imp,
                                      std::vector<size_t> &count, std::vector<std::vector<size_t>> &list);

        // add at the end
        static void addImplication(const FCA::ImplicationInd &imp,
                                   std::vector<size_t> &count, std::vector<std::vector<size_t>> &list);

        static void initCounters(const theory &L, std::vector<size_t> &count, std::vector<std::vector<size_t>> &list,
        const size_t *size = nullptr);

    };
}

# endif // FCA_LINCLOSURE_H_