#pragma once

#include <list>
#include <stack>
#include <vector>

#define debug if (0)

const int INF = 1e9;

// An edge with id.
struct EnumEdge {
  int a, b, id;
  EnumEdge() { *this = empty(); }
  EnumEdge(int _a, int _b, int _id) : a(_a), b(_b), id(_id) {}
  bool is_empty() { return id == -1; }
  static EnumEdge empty();
};

struct EdgeRange {
  EnumEdge low, high;

  EdgeRange() : low(EnumEdge::empty()), high(EnumEdge::empty()){};
  EdgeRange(EnumEdge _low, EnumEdge _high) : low(_low), high(_high){};

  bool is_empty() { return low.is_empty(); }
};

/**
 * Auxiliary struct representing edge constaint in FMR planarity test
 * algorithm. Consists of two EdgeRanges with left-edges and right-edges
 */
struct EdgeConstraint {
  EdgeRange L, R;

  EdgeConstraint() : L(), R(){};
  EdgeConstraint(EnumEdge A, EnumEdge B, EnumEdge C, EnumEdge D)
      : L(A, B), R(C, D){};

  bool is_empty() { return L.is_empty() && R.is_empty(); }
};

struct EdgeCrossRef {
  typedef std::list<std::pair<int, int>>::iterator list_it;
  list_it it1, it2;

  EdgeCrossRef() {}
  EdgeCrossRef(list_it _it1, list_it _it2) : it1(_it1), it2(_it2) {}

  list_it getCrossRef(int u) { return (it1->first == u) ? it1 : it2; }
};

/**
 * Calculate inverse of permutation [n] -> [n].
 *
 * @complexity O(n)
 * @param pi The permutation to invert.
 * @return Inverse of pi
 */
std::vector<int> get_pi_inv(std::vector<int> &pi);

/**
 * Sort adjacency list of graph according to given order.
 *
 * @complexity O(|V|^2)
 * @param list Adjacency list of the graph G.
 * @param pi The order of vertices for sorting as function [n] -> V
 */
void sort_adjacency_list(std::vector<std::vector<int>> &list,
                         std::vector<int> &pi);

/**
 * Renumerate vertices of the graph according to function phi.
 *
 * @complexity O(|V| + |E|)
 * @param list Adjacency list of the graph G.
 * @param phi Function [n] -> [n] according to which the enumeration should be
 * performed
 */
void renumerate(std::vector<std::list<int>> &list, std::vector<int> &phi);

/**
 * Enumerate edges with ids [1, m] from adjacency list maintaining vertices
 * order in each list.
 *
 * @complexity O(|V| + |E|)
 * @param list adjacency list to enumerate.
 * @return enumerated adjacency list.
 */
std::vector<std::list<std::pair<int, int>>>
enumerate_adjacency_list(const std::vector<std::list<int>> &list);

/**
 * Build edge cross-references.
 *
 * @complexity O(|V| + |E|)
 * @param elist enumerated adjacency list.
 * @param M max edge id. If equal to 0, assumed to be |E|.
 * @return vector of cross edge references.
 */
std::vector<EdgeCrossRef>
getEdgeCrossReferences(std::vector<std::list<std::pair<int, int>>> &elist,
                       int M = 0);

inline int dir(int u, int v) { return (u < v) ? 1 : -1; };

/**
 * Structure calculating bridges, articulation points and biconnected components
 * partition in O(|V| + |E|) time. Assumes vertices are numbered from [1, |V|]
 * and edges are numbered [1, |E|].
 */
class GraphConnectivity {
  std::stack<int> S;
  std::vector<bool> visited;
  std::vector<int> preorder;
  std::vector<int> low;
  int t;
  int next_bcid;

  void dfs(int v, int parent_edge);

public:
  int n, m;
  std::vector<std::list<std::pair<int, int>>>
      list;                   // adjacency list as pairs {vertex, edge_id}
  std::vector<int> p_edge_id; // id of edge to parent in dfs tree
  std::vector<bool> bridge;   // true iff edge is a bridge
  std::vector<bool> art;      // true iff vertex is an articulation point
  std::vector<int> bcid;      // biconnected component id of edge

  GraphConnectivity(std::vector<std::list<std::pair<int, int>>> _list);

  // Returns articulation points in increasing preorder.
  std::vector<int> getArticulationPoints();
};
