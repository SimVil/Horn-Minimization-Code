//
// Created by simon on 12.02.18.
//

#include "graph_FDgraph.h"


GRAPH::FDGraph::FDGraph(const std::vector<FCA::ImplicationInd> &L) { // L is not empty
    size_t attrNum = L.front().Premise().size();
    int implNum = L.size();

    FCA::BitSet premise(attrNum);
    FCA::BitSet unit(attrNum);
    FCA::BitSet conclusion(attrNum);

    for(size_t i = 0; i < attrNum; ++i)
    {
        unit.set(i);
        vertices[unit] = boost::add_vertex(compound(false, unit), fd_graph);
        unit.reset();
    }

    for(size_t i = 0; i < implNum; ++i)
    {
        premise = L[i].Premise();
        if (premise.count() > 1 && vertices.count(premise) == 0)
        {
            vertices[premise] = boost::add_vertex(compound(true, premise), fd_graph);

            for (size_t j = 0; j < attrNum; ++j)
            {
                if(premise.test(j))
                {
                    unit.set(j);
                    boost::add_edge(vertices[premise], vertices[unit], dotted(true), fd_graph);
                    unit.reset();
                }
            }
        }

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


std::vector<FCA::ImplicationInd> GRAPH::FDGraph::Convert() {
    std::vector<FCA::ImplicationInd> L;
    if (vertices.empty())
        return L;

    size_t vertNum = vertices.size();
    auto it = vertices.begin();
    size_t attrNum = it->first.size();

    FCA::BitSet premise(attrNum);
    FCA::BitSet conclusion(attrNum);

    boost::graph_traits<graph>::vertex_iterator vit, vend, vnext;
    boost::graph_traits<graph>::out_edge_iterator eit, eend, enext;
    typedef boost::graph_traits<graph>::adjacency_iterator pouet;
    pouet ait, aend, anext;

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
            {
                conclusion |= fd_graph[*ait].label;
            }

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
