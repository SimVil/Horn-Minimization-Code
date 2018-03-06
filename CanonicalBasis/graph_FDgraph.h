/// \file graph_FDgraph.h
/// \author Simon Vilmin
/// \date 2018
///
/// \brief defines FD-Graph class (FD = Functional Dependency)
///
/// The file contains an implementation of the structure given by Ausiello and al. for representing functional
/// dependency basis (hence implication system). An adjacency-list based representation is adopted to stick to
/// the theoretical work. Here are the two main papers we rely on:
///     - "Graph Algorithms for Functional Dependency Manipulation" (Ausiello and al., 1983, ACM J.)
///     - "Minimal Representation of Directed Hypergraphs" (Ausiello and al., 1986, SIAM J. Computer Science)

#ifndef ALGORITHMS_GRAPH_FDGRAPH_H
#define ALGORITHMS_GRAPH_FDGRAPH_H


#include <list>
#include <deque>
#include <map>
#include "fca_datastructures.h"



/// see struct vertex
typedef struct vertex vertex_t;


/// \typedef elt_t represents an element within a list. We store a BitSet for the label (name of the node) and an
/// vertex_t (see struct vertex).
typedef std::pair<FCA::BitSet, vertex_t> elt_t;


/// \struct defines a vertex in an FD-graph.
///
/// A node in an FD-graph can be either simple (an attribute) or a compound node (body of implication). Out of each
/// vertex, we can either have \a full or \a dotted arcs. Hence, we provide a map of edges type. Since map provide
/// access in logarithmic time (with respect to the number of list) and the number of lists being constant (up to 5,
/// see class FDGraph documentation), choosing a (key, element) setting allow for very little overhead in computation
/// as much as easier to understand coding.
///
/// \param edges map of various edges (e.g: dotted, full, ...). Keys are string naming the edge type, and elements are
///        lists of pointers on elt_t's, avoiding duplication of BitSets, while preserving easy access to adjacent node
///        labels and arcs.
/// \param counter helps to keep track of treated components of a coumpound node.
struct vertex{

    std::map<std::string, std::list<elt_t *>> edges;
    unsigned int counter;

    vertex() : edges(std::map<std::string, std::list<elt_t *>>()), counter(0) {};
    explicit vertex(unsigned int q) : counter(q) {};
};



namespace GRAPH
{
    /// \class represents an FD-Graph and its closure.
    ///
    /// This class defines an FD-Graph out of an implication system. Here is a quick recall of the structure from an
    /// example. Let L = (abc --> d) be the implication basis to model as an FD-Graph. Each attribute of L will be
    /// a \a simple node. Each body of L will be a \c compound node, here abc. Then, for a compound node, we draw a
    /// \a dotted arc from this node to each of this component. Finally, for each body, we draw a \a full arc from
    /// the related node to each of the attribute of the head. In our case, we will eventually obtain:
    ///     - simple nodes: a, b, c, d
    ///     - compound nodes: abc
    ///     - full arcs: (abc, d)
    ///     - dotted arcs: (abc, a), (abc, b), (abc, c)
    /// Then, in order to limit space complexity, and knowing that we won't add new nodes when computing the closure
    /// of a Graph, the closure edges will be embedded in the graph structure. This avoids duplicating BitSets and
    /// vertices, for the price of a very little overhead in accessing edges lists in vertex_t element (constant
    /// logarithmic time). The possible types of edges are then:
    ///     - full
    ///     - dotted
    ///     - full+: full arcs in the closure of the graph
    ///     - dotted+: dotted arcs in the closure
    ///     - D: arcs we need to compute the closure. They link attributes to compound node they belong to
    ///         (in L, we will have D_a = (a, abc)) for instance.
    /// The size of a graph is O(|L|) up to a constant factor.
    class FDGraph
    {
    protected:

        /// \brief the graph structure.
        std::list<elt_t> graph;

        /// \brief computes the closure of a node in the graph.
        ///
        /// The main point is to get, given a node, all the nodes we can reach by transitivity or by union.
        /// Transitivity is, given nodes i, j, k, having edges (i, j) and (j, k), adding (i, k) to the closure
        /// of i. Union is, given a node i, and a compound node j = j1 j2 j3..., if we have (i, j_k) for all
        /// j_k in j in the closure, then (i, j) will be also.
        ///
        /// \param v the element to compute the closure of.
        /// \param L_i initial list of nodes to treat.
        /// \param type type of closure, dotted or full.
        void NodeClosure(elt_t *v, const std::list<elt_t *> &L_i, const std::string &type);

        /// \brief searches the graph to remove edges to some node.
        ///
        /// Given a node v, we are going to run over the graph to remove all arcs (u, v). This is helpful
        /// for removing node during redundancy elimination, but also to isolate a vertex within superfluousness
        /// elimination without being compelled to remove it.
        ///
        /// \param v the node to remove
        /// \param lists the lists from which we might remove (u, v) edges.
        void removeEdgestoVertex(elt_t &v, const std::vector<std::string> &lists);

        /// \brief finds a node given its name.
        ///
        /// \param u the nme of the node (in terms of set) we want to find
        /// \return a pointer on the corresponding element.
        elt_t *findVertex(const FCA::BitSet &u);

    public:

        /// \brief default constructor;
        FDGraph() = default;

        /// \brief builds a graph out of an implication system.
        ///
        /// \param L implication system.
        explicit FDGraph(const std::vector<FCA::ImplicationInd> &L);

        /// \brief removes redundant nodes.
        ///
        /// \a requires the closure to be built. The principle is the following
        /// \code
        ///     Remove redundant nodes
        ///     Requires closure to be computed
        ///
        ///     For each compound node v:
        ///         if v has no arcs in full+:
        ///             remove edges to vertex v
        ///             remove vertex v from graph
        ///
        /// \endcode
        void RedundancyElimination();

        /// \brief remove superfluous nodes.
        ///
        /// Note: finding a superfluous node is claimed to be O(|Vc| x |V|). To reach this complexity we use the
        /// following principle:
        /// \code
        ///      For each compound node i
        ///         For each node j in the dotted closure of i
        ///             If i is in the closure of j
        ///                 i is superfluous
        ///             Else
        ///                 there is no need to go over closure of all nodes in the closure of j
        ///
        /// \endcode
        ///
        /// For the else part, here is the idea. Suppose there is a node k in the closure of j, such that i is in the
        /// closure of k. Then by transitivity, we must have i in the closure of j. Indeed we have a dotted path [i, j],
        /// a path [j, k], a path [k, i], but no path [j, i]. This is a contradiction. Logically speaking, it is the
        /// contraposition of:
        ///            {[i, j] /\ [j, k] /\ [k, i])} --> [j, i]
        /// that is
        ///            !{[j, i]} --> {!{[i, j]} \/ !{[j, k]} \/ !{[k, i]}}
        /// and for this to be true, knowing that we have [i, j] and [k, i], we must not have [j, k]. Hence, if we
        /// don't find i in the closure of j, then we won't find it in any of the node of the closure of j. That is,
        /// to know whether i is superfluous, we would have to check at most |V| nodes.
        void SuperfluousnessElimination();

        /// \brief converts a graph to an implication basis.
        ///
        /// Usually, we will need to get implications out of full arcs, but in some cases, one would get
        /// other edges to perform testing. Thus, we provide a degree of freedom on the implications we can build.
        ///
        /// \param L the resulting list.
        /// \param type the edges we would like to consider.
        void Convert(std::vector<FCA::ImplicationInd> &L, const std::string &type = "full");

        /// \brief computes the closure of the graph.
        ///
        /// This function fills two lists for each node: dotted+ and full+ for dotted closure and full closure
        /// respectively. Because dotted arcs are more likely to denote a node to be removed, we compute dotted
        /// closure first.
        void Closure();


    };
}


#endif //ALGORITHMS_GRAPH_FDGRAPH_H
