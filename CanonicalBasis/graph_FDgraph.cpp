//
// Created by simon on 12.02.18.
//

#include "graph_FDgraph.h"
#include <algorithm>


/// \brief builds a graph out of an implication system.
GRAPH::FDGraph::FDGraph(const std::vector<FCA::ImplicationInd> &L) {
    size_t implNum = L.size();
    size_t attrNum = L.front().Premise().size();

    FCA::BitSet vertex(attrNum);
    FCA::BitSet premise(attrNum);
    FCA::BitSet conclusion(attrNum);

    // vector aims to limit complexity, because access is O(1).
    std::vector<elt_t *> simple_vertex(attrNum);
    elt_t *node;


    // for each attribute, a simple node.
    for (size_t i = 0; i < attrNum; ++i)
    {
        vertex.set(i);
        graph.emplace_back(elt_t(vertex, vertex_t()));
        simple_vertex[i] = &graph.back();
        vertex.reset();
    }

    // O(|B| x |Sg|) + O(|B| x (|B| + |Sigma|)) + O(|B| x |Sg|)
    // the second stands for finding a vertex.
    // for each body, add a node, if it is compound, add dotted arcs and fill D
    // lists.
    for (size_t i = 0; i < implNum; ++i)
    {
        premise = L[i].Premise();
        conclusion = L[i].Conclusion();

        if (premise.count() != 1 && !findVertex(premise))
        {
            graph.emplace_back(elt_t(premise, vertex_t(0)));

            for(size_t j = 0; j < attrNum; ++j)
            {
                if(premise.test(j))
                {
                    // having simple_vertex helps to access an element of the
                    // list in O(1), preserving simple complexity, with a linear
                    // space augmentation (with respect to the attribute set)

                    // dotted arc from abc to a
                    // a belongs to D, hence arc from a to abc in D_a.
                    vertex.set(j);
                    graph.back().second.edges["dotted"].push_back(simple_vertex[j]);
                    simple_vertex[j]->second.edges["D"].push_back(&(graph.back()));
                    vertex.reset();
                }

            }
        }

        // findVertex is O(|B| + |Sigma|)
        node = findVertex(premise);
        for (size_t j = 0; j < attrNum; ++j)
        {
            if(conclusion.test(j))
            {
                vertex.set(j);
                node->second.edges["full"].push_back(simple_vertex[j]);
            }
        }

    }

}

/// finds a node given its name.
elt_t *GRAPH::FDGraph::findVertex(const FCA::BitSet &u) {
    auto it = graph.begin();
    elt_t * r = nullptr;
    while (it != graph.end() && it->first != u)
    {
        ++it;
    }

    if (it != graph.end())
        r = (elt_t *) &(it->first);

    return r;
}

/// computes the closure of a node in the graph.
void
GRAPH::FDGraph::NodeClosure(elt_t *i, const std::list<elt_t *> &L_i, const std::string &type)
{
    std::list<elt_t *> S_i = L_i;
    elt_t *j;
    std::string closure_type = type + "+";
    std::string dotted_closure = "dotted+";
    bool found;

    // S_i is the list of nodes we can reach from i.

    while (!S_i.empty())
    {
        j = S_i.front();
        S_i.pop_front();

        // j simple node, check for all compound node of D_j
        // to see if union rule must be applied

        if(j->first.count() == 1)
        {
            for(auto &it : j->second.edges["D"])
            {
                if(it->first != i->first)
                    it->second.counter++;

                // if counter(v) == |v|, then we can reach v from i by union
                // because we got all its components.
                if(it->second.counter == it->first.count())
                    S_i.push_back(it);
            }
        }

        i->second.edges[closure_type].push_back(j);

        // we try to perform the union of edges by splicing (constant time operation)
        // thus avoiding duplication of the next for loop.
        std::list<elt_t *> full = j->second.edges["full"];
        std::list<elt_t *> dotted = j->second.edges["dotted"];
        full.splice(full.end(), dotted);

        // for each node k with an ingoing arc from j
        // if k is neither in the closure we are computing, nor the dotted closure
        // it must be treated, by transitivity.
        for(auto it : full)
        {
            found = it->first == i->first;
            auto dotted_it = i->second.edges[dotted_closure].begin();


            while (!found && dotted_it != i->second.edges[dotted_closure].end())
            {
                found |= (*dotted_it)->first == it->first;
                ++dotted_it;
            }

            // if closure_type = dotted, we don't need to run over closure_type
            // a second time.
            if (!found && dotted_closure != closure_type)
            {
                auto full_it = i->second.edges[closure_type].begin();
                while (!found && full_it != i->second.edges[closure_type].end())
                {
                    found |= (*full_it)->first == it->first;
                    ++full_it;
                }
            }

            // if k is not in any closure, we can reach it from i, and must treat
            // it later.
            if (!found) { S_i.push_back(it); }
        }

    }

}

/// searches the graph to remove edges to some node.
void GRAPH::FDGraph::removeEdgestoVertex(elt_t &v, const std::vector<std::string> &lists) {
    size_t size;
    unsigned int i = 0;
    elt_t *elt;

    // we have to look in edges given by lists, for all vertices.
    // warning: using a pop and push strategy is valid here, since we have a fixed number
    // of elements to treat (we don't depend on the end() iterator which can be modified
    // by deletion).

    for (auto &vertex_it : graph)
    {
        for(const std::string &s : lists)
        {
            size = vertex_it.second.edges[s].size();
            i = 0;
            while (i < size)
            {
                elt = vertex_it.second.edges[s].front();
                vertex_it.second.edges[s].pop_front();

                // if the edges (vertex_it, elt) is not (vertex_t, i), we must
                // keep it, hence we push it back in the edge list.
                if(elt->first != v.first)
                    vertex_it.second.edges[s].push_back(elt);

                ++i;
            }
        }

    }

}

/// removes redundant nodes.
void GRAPH::FDGraph::RedundancyElimination() {
//    std::vector<std::string> lists = {"dotted+", "full+", "D"};
//
//    auto it = graph.begin();
//    auto tmp;
//
//    bool stop = it == graph.end();
//
//    while(!stop)
//    {
//        tmp = it;
//        ++it;
//
//        // setting the condition here prevents segmentation fault
//        // because erasing an element can alter the end() iterator.
//        // Thus, the it variable could be past the end, but not equal to the new
//        // graph.end() after destruction of an element.
//        stop = (it == graph.end());
//
//        if (tmp->first.count() != 1 && tmp->second.edges["full+"].empty())
//        {
//            removeEdgestoVertex(*tmp, lists);
//            graph.erase(tmp);
//        }
//    }

    auto it = graph.begin();
    auto tmp;
    elt_t *elt;
    int size, i;
    bool stop = it == graph.end();
    std::vector<std::string> lists = {"dotted+", "full+", "D"};

    for (auto &vertex : graph)
    {
        vertex.second.counter = (unsigned int) (vertex.first.count() != 1 && vertex.second.edges["full+"].empty());
    }

    while(!stop)
    {
        tmp = it;
        ++it;

        stop = (it == graph.end());

        // if the counter is 0, then tmp is not redundant and must remove redundant nodes from its arcs
        if (!tmp->second.counter){
            for(std::string &s : lists)
            {
                size = tmp->second.edges[s].size();
                i = 0;
                while (i < size)
                {
                    elt = tmp.second.edges[s].front();
                    tmp.second.edges[s].pop_front();

                    // if elt.counter is 0, the element pointed is not redundant and must be kept.
                    if(!elt->second.counter)
                        tmp.second.edges[s].push_back(elt);

                    ++i;
                }
            }

        } else {
            graph.erase(tmp);
        }
    }

}

/// converts a graph to an implication basis.
void GRAPH::FDGraph::Convert(std::vector<FCA::ImplicationInd> &L, const std::string &type) {
    if (graph.empty())
        return;

    L.clear();

    size_t attrNum = (graph.begin())->first.size();

    FCA::BitSet premise(attrNum);
    FCA::BitSet conclusion(attrNum);

    for(auto &vertex : graph)
    {
        premise = vertex.first;
        conclusion.reset();

        for(auto &edge : vertex.second.edges[type])
        {
            conclusion |= edge->first;
        }

        if (!conclusion.none())
            L.emplace_back(FCA::ImplicationInd(premise, conclusion));
    }

}


/// computes the closure of the graph.
void GRAPH::FDGraph::Closure()
{
    std::list<elt_t *> tmp;
    bool found;

    // WARNING: referenced iterator (otherwise, it makes a copy, and fails to reset counters
    // WARNING: some range-based loop in the following do not use referenced iterator, but this is not
    // necessary for these special iterator since we iterate over pointers, thus the pointed value is not altered
    // by copying a pointer (m, f_it).

    for(auto &vertex : graph)
    {
        for (auto &vertex_reset : graph)
            vertex_reset.second.counter = 0;

        if (!(vertex.second.edges["dotted"].empty() && vertex.second.edges["full"].empty()))
        {
            for (auto m : vertex.second.edges["D"])
            {
                m->second.counter = 1;
            }

            if(vertex.first.count() != 1)
                NodeClosure(&vertex, vertex.second.edges["dotted"], "dotted");

            // this loop determines which nodes must be treated when computing full closure
            // we perform (for a given vertex v):
            //      L_v = v["full"] - v["dotted+"]
            for (const auto full_edge : vertex.second.edges["full"])
            {
                auto closed_dotted_edge = vertex.second.edges["dotted+"].begin();
                found = false;


                while (!found && closed_dotted_edge != vertex.second.edges["dotted+"].end())
                {
                    found |= full_edge->first == (*closed_dotted_edge)->first;
                    ++closed_dotted_edge;
                }

                if (!found)
                    tmp.push_back(full_edge);

            }

            NodeClosure(&vertex, tmp, "full");
            tmp.clear();
        }
    }

}

void GRAPH::FDGraph::SuperfluousnessClosureElimination(std::list<std::pair<elt_t *, elt_t *>> &L) {
    bool superfluous;
    int treated;
    elt_t *sup;
    elt_t *tmp;
    int size;

    // O(|V|)
    for (auto &reset : graph) { reset.second.counter = 1; }

    for(auto i = graph.begin(); i != graph.end(); ++i)
    {
        sup = nullptr;
        superfluous = false;
        treated = 0;
        auto j = i->second.edges["dotted+"].begin();
        size = i->second.edges["dotted+"].size();

        for (auto &buf : i->second.edges["dotted+"]) { buf->second.counter = 0; }

        // we must be sure to treat no more than |Sigma| attribute, hence we introduce
        // counters and upper bound.
        while(!superfluous && j != i->second.edges["dotted+"].end() && treated < size){
            if ((*j)->second.counter == 0){
                std::list<elt_t *> adj_v = (*j)->second.edges["full+"];
                std::list<elt_t *> adj = (*j)->second.edges["dotted+"];
                adj_v.splice(adj_v.end(), adj);

                auto k = adj_v.begin();

                while(!superfluous && k != adj_v.end() && treated < size)
                {
                    if ((*k)->second.counter == 0)
                    {
                        superfluous = (*k)->first == i->first;
                        (*k)->second.counter = 1;
                        if (!superfluous) { treated ++; } else { sup = *k; }

                    }
                    ++k;
                }
            }
            (*j)->second.counter = 1;
            ++j;
        }

        // if i is superfluous
        if (superfluous)
        {
            for (auto &buf : i->second.edges["full+"]) { buf->second.counter = 2; }

            size = sup->second.edges["dotted+"].size();
            for (int l = 0; l < size; ++l) {
                tmp = sup->second.edges["dotted+"].front();
                sup->second.edges["dotted+"].pop_front();

                if (tmp->second.counter == 2){
                    sup->second.edges["full+"].push_back(tmp);
                } else {
                    sup->second.edges["dotted+"].push_back(tmp);
                }
            }

            removeEdgestoVertex(*i, {"dotted+", "full+"});
            i->second.edges["dotted+"].clear();
            i->second.edges["full+"].clear();
            L.push_back(std::pair<elt_t *, elt_t *>((elt_t *) &(i->first), sup));

         }

    }

}

void GRAPH::FDGraph::SuperfluousnessElimination() {
    std::list<std::pair<elt_t *, elt_t *>> L;
    std::vector<elt_t *> final_destination(graph.size(), nullptr);

    SuperfluousnessClosureElimination(L);


    unsigned int i = 1;

    // initialization  in O(|V|)
    for (auto &it : graph)
    {
        it.second.counter = i;
        ++i;
    }

    unsigned int size = L.size();

    // Determine final destination in O(|V|)
    for(i = 0; i < size; ++i)
    {
        std::pair<elt_t *, elt_t *> tmp_pair = L.back();
        L.pop_back();

        if (!final_destination[tmp_pair.second->second.counter - 1])
            final_destination[tmp_pair.second->second.counter - 1] = tmp_pair.second;

        final_destination[tmp_pair.first->second.counter - 1] = tmp_pair.second;

        L.emplace_front(tmp_pair);

    }

    // move full arcs
}
