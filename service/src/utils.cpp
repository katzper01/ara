#include "utils.hpp"
#include <vector>

/**
 * Calculate inverse of permutation [n] -> [n].
 *
 * @complexity O(n)
 * @param pi The permutation to invert.
 * @return Inverse of pi as [n] -> [n].
 */
std::vector<int> get_pi_inv(std::vector<int> &pi) {
  std::vector<int> pi_inv(pi.size(), 0);
  for (unsigned int i = 1; i < pi.size(); i++)
    pi_inv[pi[i]] = i;
  return pi_inv;
}

/**
 * Renumerate vertices of the graph according to function phi.
 *
 * @complexity O(|V| + |E|)
 * @param list Adjacency list of the graph G.
 * @param phi Function [n] -> [n] according to which the enumeration should be
 * performed
 */
void renumerate(std::vector<std::list<int>> &list, std::vector<int> &phi) {
  int n = list.size() - 1;
  std::vector<std::list<int>> _list(n + 1);
  for (int v = 1; v <= n; v++)
    for (auto u : list[v])
      _list[phi[v]].push_back(phi[u]);
  list = _list;
}

/**
 * Enumerate edges with ids [1, m] from adjacency list maintaining vertices
 * order in each list.
 *
 * @complexity O(|V| + |E|)
 * @param list adjacency list to enumerate.
 * @return enumerated adjacency list.
 */
std::vector<std::list<std::pair<int, int>>>
enumerate_adjacency_list(const std::vector<std::list<int>> &list) {
  int n = list.size() - 1;
  std::vector<std::list<std::pair<int, int>>> elist(n + 1);
  int m = 0;
  for (int v = 1; v <= n; v++) {
    for (auto &u : list[v]) {
      if (v < u) {
        m++;
        elist[u].push_back({v, m});
        elist[v].push_back({u, m});
      }
    }
  }

  std::vector<int> my_ids(n + 1);
  for (int v = 1; v <= n; v++) {
    for (auto &[u, id] : elist[v])
      my_ids[u] = id;
    elist[v].clear();
    for (auto &u : list[v])
      elist[v].push_back({u, my_ids[u]});
  }

  return elist;
}

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
                       int M) {
  int n = elist.size() - 1;

  if (M == 0) {
    for (auto &l : elist)
      M += l.size();
    M /= 2;
  }

  std::vector<EdgeCrossRef> CR(M + 1);
  for (int v = 1; v <= n; v++) {
    for (auto it = elist[v].begin(); it != elist[v].end(); ++it) {
      int u = it->first;
      int id = it->second;
      if (v < u)
        CR[id].it1 = it;
      else
        CR[id].it2 = it;
    }
  }

  return CR;
}

EnumEdge EnumEdge::empty() { return EnumEdge(-1, -1, -1); }

void GraphConnectivity::dfs(int v, int parent_edge) {
  visited[v] = true;
  preorder[v] = low[v] = ++t;
  p_edge_id[v] = parent_edge;

  int cnt_children = 0;
  for (auto &[u, id] : list[v]) {
    if (!visited[u]) {
      cnt_children++;
      S.push(id);
      dfs(u, id);
      low[v] = std::min(low[v], low[u]);
      if (low[u] >= preorder[v]) {
        art[v] = true;
        while (!S.empty()) {
          int eid = S.top();
          S.pop();
          bcid[eid] = next_bcid;
          if (eid == id)
            break;
        }
        next_bcid++;
      }
    } else if (id != parent_edge) {
      low[v] = std::min(low[v], preorder[u]);
      if (preorder[u] < preorder[v])
        S.push(id);
    }
  }

  if (v == 1)
    art[v] = (cnt_children >= 2);

  if (parent_edge != -1 && low[v] == preorder[v])
    bridge[parent_edge] = true;
}

GraphConnectivity::GraphConnectivity(
    std::vector<std::list<std::pair<int, int>>> _list)
    : list(_list) {
  n = list.size() - 1;
  m = 0;
  for (auto &l : list)
    m += l.size();
  m /= 2;

  bridge.assign(m + 1, false);
  art.assign(n + 1, false);
  bcid.assign(m + 1, 0);
  p_edge_id.assign(n + 1, 0);

  visited.assign(n + 1, false);
  low.assign(n + 1, n + 1);
  preorder.assign(n + 1, 0);

  t = 0;
  next_bcid = 1;

  dfs(1, -1);
}

// Returns articulation points in increasing preorder.
std::vector<int> GraphConnectivity::getArticulationPoints() {
  std::vector<int> preorder_inv(n + 1, 0);
  for (int v = 1; v <= n; v++)
    preorder_inv[preorder[v]] = v;
  std::vector<int> articulation_points;
  for (int i = 1; i <= n; i++)
    if (art[preorder_inv[i]])
      articulation_points.push_back(preorder_inv[i]);
  return articulation_points;
}
