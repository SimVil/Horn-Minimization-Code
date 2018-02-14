//
// Created by simon on 12.02.18.
//

#ifndef ALGORITHMS_GRAPH_FDGRAPH_H
#define ALGORITHMS_GRAPH_FDGRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include "fca_datastructures.h"
#include <deque>


namespace GRAPH {
    class FDGraph
    {
    protected:
        boost::adjacency_list<boost::listS,  // edge list (for each vertex)
                boost::listS,  // vertex (node) list
                boost::directedS, // Directed graph or not
                boost::no_property, // vertex properties (none)
                boost::property<boost::edge_weight_t, bool>, // edge properties (belongs to compound or not)
                boost::no_property, // graph properties (none)
                boost::listS // list of edges
        >  simple_part;

        boost::adjacency_list<boost::listS,  // edge list (for each vertex)
                boost::listS,  // vertex (node) list
                boost::directedS, // Directed graph or not
                boost::no_property, // vertex properties (none)
                boost::property<boost::edge_weight_t, bool>, // edge properties (dotted or full)
                boost::no_property, // graph properties (none)
                boost::listS // list of edges
        >  compound_part;

    };

}




#endif //ALGORITHMS_GRAPH_FDGRAPH_H
