//
// Created by simon on 12.02.18.
//

#include "graph_FDgraph.h"
#include <algorithm>


GRAPH::FDGraph::FDGraph(const std::vector<FCA::ImplicationInd> &L) { // L is not empty
    size_t attrNum = L.front().Premise().size();
    size_t implNum = L.size();

    FCA::BitSet premise(attrNum);
    FCA::BitSet unit(attrNum);
    FCA::BitSet conclusion(attrNum);

    // for each attribute of sigma, add a node
    for(size_t i = 0; i < attrNum; ++i)
    {
        unit.set(i);
        vertices[unit] = boost::add_vertex(compound(false, unit), fd_graph);
        unit.reset();
    }

    // for each body of L
    for(size_t i = 0; i < implNum; ++i)
    {
        premise = L[i].Premise();
        // if premise is compound and we did not add it earlier
        if (premise.count() > 1 && vertices.count(premise) == 0)
        {
            vertices[premise] = boost::add_vertex(compound(true, premise), fd_graph);

            // add dotted edge from premise to each of its attributes, and add premise to
            // corresponding D[elt]
            for (size_t j = 0; j < attrNum; ++j)
            {
                if(premise.test(j))
                {
                    unit.set(j);
                    boost::add_edge(vertices[premise], vertices[unit], dotted(true), fd_graph);
                    D[unit].push_back(vertices[premise]);
                    unit.reset();
                }
            }
        }

        // add full arcs
        conclusion = L[i].Conclusion();
        for (size_t j = 0; j < attrNum; ++j)
        {
            if(conclusion.test(j))
            {
                unit.set(j);
                boost::add_edge(vertices[premise], vertices[unit], dotted(false), fd_graph);
                unit.reset();
            }
        }

    }
}


void GRAPH::FDGraph::add_vertex(const FCA::BitSet &label, const compound &prop)
{
    vertices[label] = boost::add_vertex(prop, fd_graph);
}

GRAPH::FDGraph GRAPH::FDGraph::getEdgeEmptyCopy()
{
    GRAPH::FDGraph other;
    std::map<FCA::BitSet, graph::vertex_descriptor>::iterator it;

    for(it = vertices.begin(); it != vertices.end(); ++it)
    {
        other.add_vertex(it->first, compound(fd_graph[it->second].is_compound, fd_graph[it->second].label));
    }

    return other;
}


std::vector<FCA::ImplicationInd> GRAPH::FDGraph::Convert() {
    std::vector<FCA::ImplicationInd> L;
    if (vertices.empty())
        return L;

    auto it = vertices.begin();
    size_t attrNum = it->first.size();

    FCA::BitSet premise(attrNum);
    FCA::BitSet conclusion(attrNum);

    boost::graph_traits<graph>::vertex_iterator vit, vend, vnext;
    boost::graph_traits<graph>::out_edge_iterator eit, eend, enext;
    boost::graph_traits<graph>::adjacency_iterator ait, aend, anext;

    boost::tie(vit, vend) = boost::vertices(fd_graph);
    for (vnext = vit; vit != vend; vit = vnext)
    {
        ++vnext;
        premise = fd_graph[*vit].label;

        boost::tie(eit, eend) = boost::out_edges(*vit, fd_graph);
        boost::tie(ait, aend) = boost::adjacent_vertices(*vit, fd_graph);


        for(enext = eit; eit != eend; eit = enext)
        {
            anext = ait;
            ++enext;
            ++anext;

            if(!fd_graph[*eit].is_dotted)
                conclusion |= fd_graph[*ait].label;


            ait = anext;
        }

        if (!conclusion.none())
        {
            L.emplace_back(FCA::ImplicationInd(premise, conclusion));
            conclusion.reset();
        }
    }

    return L;

}

GRAPH::FDGraph GRAPH::FDGraph::Closure()
{
    FDGraph closure = getEdgeEmptyCopy();
    graph closure_fdgraph = closure.getGraph();
    boost::graph_traits<graph>::vertex_iterator node_b, node_e, node_it;
    boost::graph_traits<graph>::vertex_iterator c_node_b, c_node_e, c_node_it;
    std::map<FCA::BitSet, int> q_counters;
    FCA::BitSet i_node;
    FCA::BitSet m_node;

    boost::tie(node_b, node_e) = boost::vertices(fd_graph);

    for(node_it = node_b; node_b != node_e; node_b = node_it)
    {
        ++node_it;
        if(boost::out_degree(*node_b, fd_graph) > 0)
        {
            i_node = fd_graph[*node_b].label;

            if (fd_graph[*node_b].label.count() <= 1) // node i simple
            {
                for(c_node_it = c_node_b; c_node_b != c_node_e; c_node_b = c_node_it)
                {
                    ++c_node_it;
                    m_node = fd_graph[*c_node_b].label;

                    if(m_node.count() > 1)
                        // implicit cast of bool to int for initializing counters
                        q_counters[m_node] = std::find(D[i_node].begin(), D[i_node].end(), *c_node_b) != D[i_node].end();
                }
            }

            // Li0+ = 0, Li1+ = 0
            boost::clear_out_edges(*node_b, closure_fdgraph);

            if(i_node.count() > 1)
                // NodeClosure(node_b, )
        }

    }

    return closure;
}
