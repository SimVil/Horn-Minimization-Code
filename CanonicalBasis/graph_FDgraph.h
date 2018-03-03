//
// Created by simon on 12.02.18.
//

#ifndef ALGORITHMS_GRAPH_FDGRAPH_H
#define ALGORITHMS_GRAPH_FDGRAPH_H


#include <list>
#include <deque>
#include <map>
#include "fca_datastructures.h"


typedef struct vertex vertex_t;

typedef std::pair<FCA::BitSet, vertex_t> elt_t;

struct vertex{
    std::map<std::string, std::list<elt_t *>> edges;
    unsigned int counter;

    vertex() : edges(std::map<std::string, std::list<elt_t *>>()), counter(0) {};
    explicit vertex(unsigned int q) : counter(q) {};
};

namespace GRAPH
{
    class FDGraph
    {
    protected:

        std::list<elt_t> graph;
        void NodeClosure(elt_t *v, const std::list<elt_t *> &L_i, const std::string &type);

        void removeEdgestoVertex(elt_t &v, const std::vector<std::string> &lists);

        elt_t *findVertex(const FCA::BitSet &u);

    public:
        FDGraph() = default;
        explicit FDGraph(const std::vector<FCA::ImplicationInd> &L);

        void RedundancyElimination();

        void Convert(std::vector<FCA::ImplicationInd> &L, const std::string &type = "full");

        void Closure();


    };
}


#endif //ALGORITHMS_GRAPH_FDGRAPH_H
