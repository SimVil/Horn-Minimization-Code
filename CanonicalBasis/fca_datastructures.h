/* ---------------------------------------------------------------------------------------------------------------------
 * FILE:   fca_datastructures.h
 * AUTHOR: Konstantin Bazhanov
 * DATE:   2012
 *
 * Last Modified: 2018
 * By: Simon Vilmin
 *
 * This file aims to summarize various data structures defined for FCA computations:
 *  - implications,
 *  - context,
 *  - concept.
 *  It lightens headers inclusions when data structures are needed.
 * ------------------------------------------------------------------------------------------------------------------ */


# pragma once

# ifndef FCA_DATASTRUCTURES_H_
# define FCA_DATASTRUCTURES_H_

# include "fca_context.h"
# include "fca_concept.h"
# include "fca_implications.h"

typedef std::vector<FCA::ImplicationInd> theory;

# endif //FCA_DATASTRUCTUERS_H_