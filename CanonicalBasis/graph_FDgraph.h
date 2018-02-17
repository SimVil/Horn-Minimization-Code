//
// Created by simon on 12.02.18.
//

#ifndef ALGORITHMS_GRAPH_FDGRAPH_H
#define ALGORITHMS_GRAPH_FDGRAPH_H

#include "fca_datastructures.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>


struct compound
{
    bool is_compound;
    FCA::BitSet label;
    compound(bool v, const FCA::BitSet &name) : is_compound(v), label(name) {};
};

struct dotted
{
    bool is_dotted;
    explicit dotted(bool v) : is_dotted(v) {};
};

typedef boost::adjacency_list<boost::vecS,  // edge list (for each vertex)
        boost::listS,  // vertex (node) list
        boost::directedS, // Directed graph or not
        compound, // vertex properties (compound or not)
        dotted, // edge properties (dotted or not)
        boost::no_property, // graph properties (none)
        boost::listS // list of edges
>  graph;

namespace GRAPH {
    class FDGraph
    {
    protected:
        graph fd_graph;

        std::map<FCA::BitSet, graph::vertex_descriptor> vertices;

    public:
        FDGraph(const std::vector<FCA::ImplicationInd> &L);

        std::vector<FCA::ImplicationInd> Convert();

    };


}




#endif //ALGORITHMS_GRAPH_FDGRAPH_H
