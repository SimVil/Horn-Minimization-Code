//
// Created by simon on 12.02.18.
//

#include "graph_FDgraph.h"
#include <algorithm>


GRAPH::FDGraph::FDGraph(const std::vector<FCA::ImplicationInd> &L)
{
    size_t implNum = L.size();
    size_t attrNum = L.front().Premise().size();

    FCA::BitSet vertex(attrNum);
    FCA::BitSet premise(attrNum);
    FCA::BitSet conclusion(attrNum);

    for(size_t i = 0; i < attrNum; ++i)
    {
        vertex.set(i);
        graph.insert(std::pair<FCA::BitSet, vertex_t>(vertex, vertex_t()));
        vertex.reset();
    }

    for(size_t i = 0; i < implNum; ++i)
    {
        premise = L[i].Premise();
        conclusion = L[i].Conclusion();

        if (premise.count() > 1 && graph.find(premise) == graph.end())
        {
            graph.insert(std::pair<FCA::BitSet, vertex_t>(premise, vertex_t(0)));

            for(size_t j = 0; j < attrNum; ++j)
            {
                if (premise.test(j))
                {
                    vertex.set(j);
                    graph[premise].edges["dotted"].push_back(&(graph.find(vertex)->first));
                    graph[vertex].edges["D"].push_back(&(graph.find(premise)->first));
                    vertex.reset();
                }
            }
        }

        for (size_t j = 0; j < attrNum; ++j)
        {
            if (conclusion.test(j))
            {
                vertex.set(j);
                graph[premise].edges["full"].push_back(&(graph.find(vertex)->first));
                vertex.reset();
            }
        }

    }

}


void GRAPH::FDGraph::Convert(std::vector<FCA::ImplicationInd> &L, const std::string &type) {
    if (graph.empty())
        return;

    // /!\ Reset the implicational system /!\.
    L.clear();

    size_t attrNum = (graph.begin())->first.size();

    FCA::BitSet premise(attrNum);
    FCA::BitSet conclusion(attrNum);

    for(auto it : graph)
    {
        premise = it.first;
        conclusion.reset();

        for(auto it_full : graph[premise].edges[type])
        {
            conclusion |= *it_full;
        }

        if (!conclusion.none())
            L.emplace_back(FCA::ImplicationInd(premise, conclusion));
    }

}


void
GRAPH::FDGraph::NodeClosure(const FCA::BitSet &i, const std::list<const FCA::BitSet *> &L_i,
                            const std::string &type)
{
    if (graph.find(i) == graph.end())
        return;

    std::list<const FCA::BitSet *> S_i = L_i;
    const FCA::BitSet *j;
    std::string closure_type = type + "+";
    std::string dotted_closure = "dotted+";
    bool found = false;

    while (!S_i.empty())
    {
        j = S_i.front();
        S_i.pop_front();

        if (j->count() == 1)
        {
            for(auto it : graph[*j].edges["D"])
            {
                if(*it != i)
                    graph[*it].counter++;

                if (graph[*it].counter == it->count())
                    S_i.push_back(it);
            }
        }

        graph[i].edges[closure_type].push_back(j);

        // we try to perform the union of edges by splicing (constant time operation)
        // thus avoiding duplication of the loop.

        std::list<const FCA::BitSet *> full = graph[*j].edges["full"];
        std::list<const FCA::BitSet *> dotted = graph[*j].edges["dotted"];
        full.splice(full.end(), dotted);

        for(auto it : full)
        {
            found = *it == i;
            auto dotted_it = graph[i].edges[dotted_closure].begin();

            while (!found && dotted_it != graph[i].edges[dotted_closure].end())
            {
                found |= **dotted_it == *it;
                ++dotted_it;
            }

            if (!found && dotted_closure != closure_type)
            {
                auto full_it = graph[i].edges[closure_type].begin();
                while (!found && full_it != graph[i].edges[closure_type].end())
                {
                    found |= **full_it == *it;
                    ++full_it;
                }
            }

            if (!found) { S_i.push_back(it); }
        }

    }

}


void GRAPH::FDGraph::Closure()
{
    std::list<const FCA::BitSet *> tmp;
    bool found = false;

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
                graph[*m].counter = 1;
            }

            if(it.first.count() != 1)
                NodeClosure(it.first, it.second.edges["dotted"], "dotted");

            for (const auto f_it : it.second.edges["full"])
            {
                auto d_it = it.second.edges["dotted+"].begin();
                found = false;

                while (!found && d_it != it.second.edges["dotted+"].end())
                {
                    found |= *f_it == **d_it;
                    ++d_it;
                }

                if (!found)
                    tmp.push_back(f_it);

            }

            NodeClosure(it.first, tmp, "full");
            tmp.clear();
        }
    }
    
}

void GRAPH::FDGraph::removeVertex(const FCA::BitSet &u, const std::vector<std::string> &lists) {
    unsigned int size;
    unsigned int i = 0;
    const FCA::BitSet *elt;

    for (auto &vertex_it : graph)
    {
        for(const std::string &s : lists)
        {
            size = graph[vertex_it.first].edges[s].size();
            i = 0;
            while (i < size)
            {
                elt = graph[vertex_it.first].edges[s].front();
                graph[vertex_it.first].edges[s].pop_front();

                if(*elt != u)
                    graph[vertex_it.first].edges[s].push_back(elt);

                ++i;
            }
        }
    }

    auto it = graph.find(u);
    graph.erase(it);
    std::cout << "Pouet" << std::endl;
}

void GRAPH::FDGraph::RedundancyElimination() {
    std::vector<std::string> lists = {"dotted+", "full+", "D"};
    FCA::BitSet tmp;
    for (auto it = graph.begin(); it != graph.end(); ++it)
    {
        if(it->first.count() != 1 && it->second.edges["full+"].empty())
        {
            tmp = it->first;
            ++it;
            removeVertex(tmp, lists);
        }

    }
}





//GRAPH::FDGraph::FDGraph(const std::vector<FCA::ImplicationInd> &L) { // L is not empty
//    size_t attrNum = L.front().Premise().size();
//    size_t implNum = L.size();
//
//    FCA::BitSet premise(attrNum);
//    FCA::BitSet unit(attrNum);
//    FCA::BitSet conclusion(attrNum);
//
//    // for each attribute of sigma, add a node
//    for(size_t i = 0; i < attrNum; ++i)
//    {
//        unit.set(i);
//        vertices[unit] = boost::add_vertex(compound(false, unit), fd_graph);
//        unit.reset();
//    }
//
//    // for each body of L
//    for(size_t i = 0; i < implNum; ++i)
//    {
//        premise = L[i].Premise();
//        // if premise is compound and we did not add it earlier
//        if (premise.count() > 1 && vertices.count(premise) == 0)
//        {
//            vertices[premise] = boost::add_vertex(compound(true, premise), fd_graph);
//
//            // add dotted edge from premise to each of its attributes, and add premise to
//            // corresponding D[elt]
//            for (size_t j = 0; j < attrNum; ++j)
//            {
//                if(premise.test(j))
//                {
//                    unit.set(j);
//                    boost::add_edge(vertices[premise], vertices[unit], dotted(true), fd_graph);
//                    D[unit].push_back(vertices[premise]);
//                    unit.reset();
//                }
//            }
//        }
//
//        // add full arcs
//        conclusion = L[i].Conclusion();
//        for (size_t j = 0; j < attrNum; ++j)
//        {
//            if(conclusion.test(j))
//            {
//                unit.set(j);
//                boost::add_edge(vertices[premise], vertices[unit], dotted(false), fd_graph);
//                unit.reset();
//            }
//        }
//
//    }
//}
//
//
//std::vector<FCA::ImplicationInd> GRAPH::FDGraph::Convert() {
//    std::vector<FCA::ImplicationInd> L;
//    if (vertices.empty())
//        return L;
//
//    auto it = vertices.begin();
//    size_t attrNum = it->first.size();
//
//    FCA::BitSet premise(attrNum);
//    FCA::BitSet conclusion(attrNum);
//
//    boost::graph_traits<graph>::vertex_iterator vit, vend, vnext;
//    boost::graph_traits<graph>::out_edge_iterator eit, eend, enext;
//    boost::graph_traits<graph>::adjacency_iterator ait, aend, anext;
//
//    boost::tie(vit, vend) = boost::vertices(fd_graph);
//    for (vnext = vit; vit != vend; vit = vnext)
//    {
//        ++vnext;
//        premise = fd_graph[*vit].label;
//
//        boost::tie(eit, eend) = boost::out_edges(*vit, fd_graph);
//        boost::tie(ait, aend) = boost::adjacent_vertices(*vit, fd_graph);
//
//
//        for(enext = eit; eit != eend; eit = enext)
//        {
//            anext = ait;
//            ++enext;
//            ++anext;
//
//            if(!fd_graph[*eit].is_dotted)
//                conclusion |= fd_graph[*ait].label;
//
//
//            ait = anext;
//        }
//
//        if (!conclusion.none())
//        {
//            L.emplace_back(FCA::ImplicationInd(premise, conclusion));
//            conclusion.reset();
//        }
//    }
//
//    return L;
//
//}

//GRAPH::FDGraph GRAPH::FDGraph::Closure() {
//    return GRAPH::FDGraph();
//}

//GRAPH::FDGraph GRAPH::FDGraph::Closure()
//{
//    FDGraph closure = getEdgeEmptyCopy();
//    graph closure_fdgraph = closure.getGraph();
//    boost::graph_traits<graph>::vertex_iterator node_b, node_e, node_it;
//    boost::graph_traits<graph>::vertex_iterator c_node_b, c_node_e, c_node_it;
//    std::map<FCA::BitSet, int> q_counters;
//    FCA::BitSet i_node;
//    FCA::BitSet m_node;
//
//    boost::tie(node_b, node_e) = boost::vertices(fd_graph);
//
//    for(node_it = node_b; node_b != node_e; node_b = node_it)
//    {
//        ++node_it;
//        if(boost::out_degree(*node_b, fd_graph) > 0)
//        {
//            i_node = fd_graph[*node_b].label;
//
//            if (fd_graph[*node_b].label.count() <= 1) // node i simple
//            {
//                for(c_node_it = c_node_b; c_node_b != c_node_e; c_node_b = c_node_it)
//                {
//                    ++c_node_it;
//                    m_node = fd_graph[*c_node_b].label;
//
//                    if(m_node.count() > 1)
//                        // implicit cast of bool to int for initializing counters
//                        q_counters[m_node] = std::find(D[i_node].begin(), D[i_node].end(), *c_node_b) != D[i_node].end();
//                }
//            }
//
//            // Li0+ = 0, Li1+ = 0
//            boost::clear_out_edges(*node_b, closure_fdgraph);
//
//            if(i_node.count() > 1)
//                // NodeClosure(node_b, )
//        }
//
//    }
//
//    return closure;
//}
