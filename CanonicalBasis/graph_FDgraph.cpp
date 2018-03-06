//
// Created by simon on 12.02.18.
//

#include "graph_FDgraph.h"
#include <algorithm>


GRAPH::FDGraph::FDGraph(const std::vector<FCA::ImplicationInd> &L) {
    size_t implNum = L.size();
    size_t attrNum = L.front().Premise().size();

    FCA::BitSet vertex(attrNum);
    FCA::BitSet premise(attrNum);
    FCA::BitSet conclusion(attrNum);

    std::vector<elt_t *> simple_vertex(attrNum);
    elt_t *node;


    // O(|Sigma|)
    for (size_t i = 0; i < attrNum; ++i)
    {
        vertex.set(i);
        graph.emplace_back(elt_t(vertex, vertex_t()));
        simple_vertex[i] = &graph.back();
        vertex.reset();
    }

    // O(|B| x |Sg|) + O(|B| x (|B| + |Sigma|)) + O(|B| x |Sg|)
    // the second stands for finding a vertex.
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
                    vertex.set(j);
                    graph.back().second.edges["dotted"].push_back(simple_vertex[j]);
                    simple_vertex[j]->second.edges["D"].push_back(&(graph.back()));
                    vertex.reset();
                }

            }
        }

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

void
GRAPH::FDGraph::NodeClosure(elt_t *i, const std::list<elt_t *> &L_i, const std::string &type)
{
    std::list<elt_t *> S_i = L_i;
    elt_t *j;
    std::string closure_type = type + "+";
    std::string dotted_closure = "dotted+";
    bool found;

    while (!S_i.empty())
    {
        j = S_i.front();
        S_i.pop_front();

        if(j->first.count() == 1)
        {
            for(auto &it : j->second.edges["D"])
            {
                if(it->first != i->first)
                    it->second.counter++;

                if(it->second.counter == it->first.count())
                    S_i.push_back(it);
            }
        }

        i->second.edges[closure_type].push_back(j);

        // we try to perform the union of edges by splicing (constant time operation)
        // thus avoiding duplication of the loop.

        std::list<elt_t *> full = j->second.edges["full"];
        std::list<elt_t *> dotted = j->second.edges["dotted"];
        full.splice(full.end(), dotted);

        for(auto it : full)
        {
            found = it->first == i->first;
            auto dotted_it = i->second.edges[dotted_closure].begin();

            while (!found && dotted_it != i->second.edges[dotted_closure].end())
            {
                found |= (*dotted_it)->first == it->first;
                ++dotted_it;
            }

            if (!found && dotted_closure != closure_type)
            {
                auto full_it = i->second.edges[closure_type].begin();
                while (!found && full_it != i->second.edges[closure_type].end())
                {
                    found |= (*full_it)->first == it->first;
                    ++full_it;
                }
            }

            if (!found) { S_i.push_back(it); }
        }

    }

}


void GRAPH::FDGraph::removeEdgestoVertex(elt_t &v, const std::vector<std::string> &lists) {
    size_t size;
    unsigned int i = 0;
    elt_t *elt;

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

                if(elt->first != v.first)
                    vertex_it.second.edges[s].push_back(elt);

                ++i;
            }
        }

    }

}

void GRAPH::FDGraph::RedundancyElimination() {
    std::vector<std::string> lists = {"dotted+", "full+", "D"};

    auto it = graph.begin();
    auto tmp;

    bool stop = it == graph.end();

    while(!stop)
    {
        tmp = it;
        ++it;

        stop = (it == graph.end());

        if (tmp->first.count() != 1 && tmp->second.edges["full+"].empty())
        {
            removeEdgestoVertex(*tmp, lists);
            graph.erase(tmp);
        }
    }
}

void GRAPH::FDGraph::Convert(std::vector<FCA::ImplicationInd> &L, const std::string &type) {
    if (graph.empty())
        return;

    L.clear();

    size_t attrNum = (graph.begin())->first.size();

    FCA::BitSet premise(attrNum);
    FCA::BitSet conclusion(attrNum);

    for(auto &it : graph)
    {
        premise = it.first;
        conclusion.reset();

        for(auto &it_full : it.second.edges[type])
        {
            conclusion |= it_full->first;
        }

        if (!conclusion.none())
            L.emplace_back(FCA::ImplicationInd(premise, conclusion));
    }

}

void GRAPH::FDGraph::Closure()
{
    std::list<elt_t *> tmp;
    bool found;

    // WARNING: referenced iterator (otherwise, it makes a copy, and fails to reset counters
    // WARNING: some range-based loop in the following do not use referenced iterator, but this is not
    // necessary for these special iterator since we iterate over pointers, thus the pointed value is not altered
    // by copying a pointer (m, f_it).

    for(auto &it : graph)
    {
        for (auto &it_b : graph)
            it_b.second.counter = 0;

        if (!(it.second.edges["dotted"].empty() && it.second.edges["full"].empty()))
        {
            for (auto m : it.second.edges["D"])
            {
                m->second.counter = 1;
            }

            if(it.first.count() != 1)
                NodeClosure(&it, it.second.edges["dotted"], "dotted");

            for (const auto f_it : it.second.edges["full"])
            {
                auto d_it = it.second.edges["dotted+"].begin();
                found = false;

                while (!found && d_it != it.second.edges["dotted+"].end())
                {
                    found |= f_it->first == (*d_it)->first;
                    ++d_it;
                }

                if (!found)
                    tmp.push_back(f_it);

            }

            NodeClosure(&it, tmp, "full");
            tmp.clear();
        }
    }

}


void GRAPH::FDGraph::SuperfluousnessElimination() {
    auto i;

    for(i = graph.begin(); i != graph.end(); ++i)
    {
        for(auto &i_dp : i->second.edges["dotted+"])
        {

            std::list<elt_t *> adj_v = i_dp->second.edges["full+"];
            std::list<elt_t *> adj = i_dp->second.edges["dotted+"];
            adj_v.splice(adj_v.end(), adj);

            auto j = adj_v.begin();

            while(j != adj_v.end() && (*j)->first != i->first) { ++j; }

            // if i is superfluous
            if (j != adj_v.end())
            {
                
                removeEdgestoVertex(*i, {"dotted+", "full+"});
            }
        }
    }
}


