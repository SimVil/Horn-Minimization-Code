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

        // for each node k with an ingoing arc from j
        // if k is neither in the closure we are computing, nor the dotted closure
        // it must be treated, by transitivity.

        // default behaviour to run across full AND dotted arcs of some point
        auto it = j->second.edges["dotted"].begin();

        // if full list is not empty, we have to analyze it
        if (!j->second.edges["full"].empty())
            it = j->second.edges["full"].begin();

        while(it != j->second.edges["dotted"].end())
        {
            found = (*it)->first == i->first;
            auto dotted_it = i->second.edges[dotted_closure].begin();


            while (!found && dotted_it != i->second.edges[dotted_closure].end())
            {
                found |= (*dotted_it)->first == (*it)->first;
                ++dotted_it;
            }

            // if closure_type = dotted, we don't need to run over closure_type
            // a second time.
            if (!found && dotted_closure != closure_type)
            {
                auto full_it = i->second.edges[closure_type].begin();
                while (!found && full_it != i->second.edges[closure_type].end())
                {
                    found |= (*full_it)->first == (*it)->first;
                    ++full_it;
                }
            }

            // if k is not in any closure, we can reach it from i, and must treat
            // it later.
            if (!found) { S_i.push_back(*it); }
            ++it;

            // we ended the full list, beginning the dotted one.
            if (it == j->second.edges["full"].end())
                it = j->second.edges["dotted"].begin();
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
    auto it = graph.begin();
    elt_t *elt;
    int size, i;
    bool stop;
    std::vector<std::string> lists = {"dotted+", "full+", "D"};

    // step 1: marking redundant nodes O(|V|)
    for (auto &vertex : graph)
    {
        vertex.second.counter = (unsigned int) (!vertex.second.edges["dotted+"].empty()
                                                && vertex.second.edges["full+"].empty());
    }

    // step 2: removing arcs to marked nodes. O(|E|) = O(|B|^2 + |L|)
    for (auto &v : graph)
    {
        // if the counter is 0, v is not redundant and we must remove redundant nodes from its arcs
        if (!v.second.counter){
            for(std::string &s : lists)
            {
                size = v.second.edges[s].size();
                i = 0;
                while (i < size)
                {
                    elt = v.second.edges[s].front();
                    v.second.edges[s].pop_front();

                    // if elt.counter is 0, the element pointed is not redundant and must be kept.
                    if(!elt->second.counter)
                        v.second.edges[s].push_back(elt);

                    ++i;
                }
            }

        } else {
            v.second.edges["dotted+"].clear();
            v.second.edges["full+"].clear();
        }
    }

    // step 3: removing marked nodes : O(|B|)
    it = graph.begin();
    stop = it == graph.end();
    while(!stop)
    {
        auto tmp = it;
        ++it;

        stop = it == graph.end();

        if(tmp->second.counter)
            graph.erase(tmp);
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

        for(auto edge : vertex.second.edges[type]) {
            conclusion |= edge->first; }

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
            for (auto m : vertex.second.edges["D"]) { m->second.counter = 1; }

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
    int treated, size, i;
    elt_t *sup, *tmp, *elt;

    // O(|V|)
    for (auto &reset : graph) { reset.second.counter = 1; }

    // remark : a node is compound if and only if its dotted closure is not empty (because it contains at least
    // its single attributes)

    // Considering not entering the if as a 0 cost operation, we wil run only over compound nodes : the outter loop
    // is O(|Vc|). Then, we use a marking procedure similar to:
    //      for a given node j in vertex["dotted+"], if vertex is not in j["full+"] + j["dotted+"] then we
    //      mark all vertices in j["full+"] + j["dotted+"] as "not to be treated"
    // This reduces from 0 to |j["full+"] + j["dotted+"]| the number of vertices for which operations must be performed
    // limiting to |V| (times +/- |V| testing conditions in O(0) under assumption) operations. Thus, finding a node j
    // equivalent to vertex is O(|V|) operations.
    // If we find vertex to be superfluous (wrt sup), we mark all elements in vertex["full+"] (O(|V|)). Then, for all
    // vertices in sup["dotted+"], if we find a marked vertex, we move it to sup["full+"] (see algorithm). Because we
    // are working on lists, the moving operations are O(1). Hence, moving arcs is also O(|V|) operations.

    // Overall complexity of this loop : O(|Vc| x |V|)
    for(auto &vertex : graph)
    {
        // if the vertex is compound
        if (!vertex.second.edges["dotted+"].empty())
        {
            sup = nullptr;
            superfluous = false;
            treated = 0;
            auto j = vertex.second.edges["dotted+"].begin();
            size = vertex.second.edges["dotted+"].size();

            for (auto buf : vertex.second.edges["dotted+"]) { buf->second.counter = 0; }

            // to limit computations, we try to perform only "interesting computations". A node in vertex["dotted+"]
            // will not be treated if we already met it previously without finding superfluousness.
            // Also, elements not being in vertex["dotted+"] will not be considered in the nested loop.
            while(!superfluous && j != vertex.second.edges["dotted+"].end() && treated < size){
                if ((*j)->second.counter == 0){


                    // default behavior
                    auto k = (*j)->second.edges["dotted+"].begin();

                    // full+ may be empty only if j is a simple node without outgoing arcs
                    if (!(*j)->second.edges["full+"].empty())
                        k = (*j)->second.edges["full+"].begin();


                    while(!superfluous && k != (*j)->second.edges["dotted+"].end() && treated < size)
                    {
                        if ((*k)->second.counter == 0 || (*k)->first == vertex.first)
                        {

                            superfluous = (*k)->first == vertex.first;

                            (*k)->second.counter = 1;
                            if (!superfluous) { treated ++; } else { sup = *j; }

                        }
                        ++k;

                        // change of lists. If we finished the first one, we jump to the second one
                        // they are disjoint (closure).
                        if (k == (*j)->second.edges["full+"].end())
                            k = (*j)->second.edges["dotted+"].begin();
                    }
                }
                (*j)->second.counter = 1;
                ++j;
            }

            // if vertex is superfluous
            if (superfluous)
            {
                for (auto &buf : vertex.second.edges["full+"]) { buf->second.counter = 2; }

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

                //removeEdgestoVertex(*i, {"dotted+", "full+"});
                // because we clear out superfluous nodes arcs, they will no longer appear as a replacement
                // for other nodes
                // Issue: in the loop transforming arcs, we may transform obsolete arcs --> useless computation
                //         but less time-consuming than removeEdgestoVertex.
                vertex.second.edges["dotted+"].clear();
                vertex.second.edges["full+"].clear();
                L.emplace_back(std::pair<elt_t *, elt_t *>(&vertex, sup));

            }
        }

    }

    // removing all closure arcs going into superfluous nodes
    for (auto &reset : graph) { reset.second.counter = 0; } // O(|V|)
    for (auto &it : L) { it.first->second.counter = 1; }  // O(|B|)

    // if we consider the iteration outside the if(!it.second.counter) to be very very little overhead, the
    // loop runs in O(|B|^2 + |L|) = O(|B| x (|B| + |Sg|)) = O(|Vc| x |V|)
    for(auto &it : graph)
    {
        // if counter is 0, vertex "it" is not superfluous: we remove closure arcs to superfluous node
        if (!it.second.counter)
        {
            for(std::string &s : (std::vector<std::string>) {"dotted+", "full+"})
            {
                size = it.second.edges[s].size();
                i = 0;

                while (i < size)
                {
                    elt = it.second.edges[s].front();
                    it.second.edges[s].pop_front();

                    // if elt.counter is 0, the element pointed is not superfluous and must be kept.
                    if(!elt->second.counter)
                        it.second.edges[s].push_back(elt);

                    ++i;
                }
            }
        }
    }

    // Overall complexity of these steps : O(|Vc||V|) as mentioned in the algorithm.
    // Possible issues: we were compelled to add several linear loop to use counter tricks to flatten complexity,
    //                  code remains to be tested for now.

}

void GRAPH::FDGraph::SuperfluousnessElimination() {

    if (graph.empty())
        return;

    std::list<std::pair<elt_t *, elt_t *>> L;
    std::vector<elt_t *> fl_dest(graph.size(), nullptr);
    elt_t *tmp;
    // because attribute nodes are created first, there will be no indexing issue with existing_edge
    std::vector<bool> existing_edges(graph.front().first.size(), false);

    SuperfluousnessClosureElimination(L);

    unsigned int i = 0;
    unsigned int size = L.size();

    // initialization  in O(|V|)
    for (auto &it : graph) {
        it.second.counter = i;
        ++i;
    }

    // Determine final destination in O(|V|) = O(|B| + |Sg|)
    for(i = 0; i < size; ++i)
    {
        std::pair<elt_t *, elt_t *> tmp_pair = L.back();
        L.pop_back();

        if (!fl_dest[tmp_pair.second->second.counter])
            fl_dest[tmp_pair.second->second.counter] = tmp_pair.second;

        fl_dest[tmp_pair.first->second.counter] = tmp_pair.second;

        L.emplace_front(tmp_pair);

    }

    // move full arcs: outer loop is O(|B|). The whole loop runs in O(|B|.4.|Sg|) = O(|L|) = O(|E|) where
    // E is limited to dotted and full arcs (as in the theoretical algorithm).
    for(auto &v_pair : L)
    {
        size = v_pair.first->second.edges["full"].size();

        // O(|Sg|)
        // v is forced to have at least one full arc, it would be removed by redundancy elimination otherwise.
        auto v = fl_dest[v_pair.first->second.counter]->second.edges["full"].begin();
        while(v != fl_dest[v_pair.first->second.counter]->second.edges["dotted"].end())
        {
            existing_edges[(*v)->second.counter] = true;
            ++v;
            if (v == fl_dest[v_pair.first->second.counter]->second.edges["full"].end())
                v = fl_dest[v_pair.first->second.counter]->second.edges["dotted"].begin();
        }

        // O(|Sg|): for all full arc (i, k) we are supposed to move to j, we first check that (j, k) is not
        // already present in j["dotted"] and j["full"]. This avoids duplication.

        //FCA::BitSet pouet;
        for(i = 0; i < size; ++i)
        {
            tmp = v_pair.first->second.edges["full"].front();
            v_pair.first->second.edges["full"].pop_front();

            if(!existing_edges[tmp->second.counter]){
                fl_dest[v_pair.first->second.counter]->second.edges["full"].emplace_back(tmp);

            }



        }

        // O(|Sg|)
        std::fill(existing_edges.begin(), existing_edges.end(), false);

        // O(|Sg|)
        v_pair.first->second.edges["dotted"].clear();
    }

    // remove arcs with superfluous end : O(|Sg|.|B|) = O(|L|) = O(|E|)
    for (auto &v : graph)
    {
        size = v.second.edges["D"].size();
        i = 0;

        while (i < size)
        {
            tmp = v.second.edges["D"].front();
            v.second.edges["D"].pop_front();

            // if fl_dest[j] is null or fl_dest[j] = j, j is not superfluous --> keep the edge?
            if (!fl_dest[tmp->second.counter] || fl_dest[tmp->second.counter] == tmp)
                v.second.edges["D"].emplace_back(tmp);

            ++i;
        }
    }


    // delete superfluous nodes: O(|B|)
    auto v = graph.begin();
    bool stop;
    FCA::BitSet pouet;

    do {
        auto elt = v;
        pouet = elt->first;
        ++v;
        stop = (v == graph.end());

        if (fl_dest[elt->second.counter] && fl_dest[elt->second.counter] != (elt_t *) &(elt->first))
            graph.erase(elt);

    } while (!stop);

}

