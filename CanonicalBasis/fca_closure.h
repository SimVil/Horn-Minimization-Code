# pragma once

# ifndef FCA_CLOSURE_H_
# define FCA_CLOSURE_H_

# include "fca_datastructures.h"

namespace FCA
{
    class Closure
    {	
    public:			
        static bool Apply(const BitSet &current, const theory &implications, BitSet &res,
                          const size_t *prefLen = nullptr, BitSet *Implied = nullptr, size_t *reach = nullptr, bool stop = false);
    };
}

# endif // FCA_CLOSURE_H_