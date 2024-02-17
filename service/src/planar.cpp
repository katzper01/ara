#include "utils.hpp"
#include <algorithm>
#include <vector>

void update_low(int &low, int &low2, int x) {
  if (x < low) {
    low2 = low;
    low = x;
  } else if (x > low && x < low2)
    low2 = x;
}

/** Calculates several things for searched graph.
 * - preorder[v] of dfs search
 * - low[v] function (standard definition with preorders)
 * - low2[v] second best candidate for low[v] (low[v] = low2[v] iff there is
 *     only one candidate for low[v] - v itself)
 * - T set of edges in DFS orientation (tree + backwards edges)
 */
void dfs1(int v, int f, std::vector<bool> &visited, int &last_preorder,
          std::vector<std::list<int>> &list, std::vector<int> &preorder,
          std::vector<int> &low, std::vector<int> &low2,
          std::vector<std::pair<int, int>> &T) {
  visited[v] = true;
  preorder[v] = ++last_preorder;
  low[v] = low2[v] = preorder[v];
  for (auto u : list[v]) {
    if (!visited[u]) { // tree edge
      T.push_back({v, u});
      dfs1(u, v, visited, last_preorder, list, preorder, low, low2, T);
      update_low(low[v], low2[v], low[u]);
      update_low(low[v], low2[v], low2[u]);
    } else if (u != f && preorder[v] > preorder[u]) { // back edge
      T.push_back({v, u});
      update_low(low[v], low2[v], preorder[u]);
    }
  }
}

/* Returns true iff edge (v, u) is a tree edge (otherwise it's backwards edge).
 */
inline bool is_T(std::pair<int, int> e) { return e.first < e.second; }

inline bool conflict(EdgeRange &I, EnumEdge &e, std::vector<int> &low_e) {
  return !I.is_empty() && low_e[I.high.id] > low_e[e.id];
}

EdgeRange merge_edge_ranges(EdgeRange &A, EdgeRange B,
                            std::vector<EnumEdge> &ref,
                            std::vector<int> &side) {
  if (A.is_empty())
    return B;
  if (!B.is_empty()) {
    ref[B.low.id] = A.high;
    side[B.low.id] = 1;
  }
  if (B.is_empty())
    B.high = A.high;
  B.low = A.low;
  return B;
}

/* Subprocedure of dfs2, returns false if graph is detected to not be planar. */
bool merge(EnumEdge &e, EnumEdge &e_i, int bottom, std::vector<int> &low_e,
           std::vector<EnumEdge> &low_edge, std::vector<EnumEdge> &ref,
           std::vector<int> &side, std::vector<EdgeConstraint> &S) {
  EdgeConstraint P;
  while ((int)S.size() - 1 > bottom) {
    EdgeConstraint q = S.back();
    S.pop_back();
    if (!q.L.is_empty() && !q.R.is_empty())
      return false;
    if (!q.L.is_empty())
      std::swap(q.L, q.R);

    if (low_e[q.R.low.id] == low_e[e.id]) {
      ref[q.R.low.id] = low_edge[e.id];
      side[q.R.low.id] = 1;
    } else
      P.R = merge_edge_ranges(q.R, P.R, ref, side);
  }

  while (!S.empty() && (conflict(S.back().L, e_i, low_e) ||
                        conflict(S.back().R, e_i, low_e))) {
    EdgeConstraint q = S.back();
    S.pop_back();

    if (conflict(q.R, e_i, low_e))
      std::swap(q.L, q.R);
    if (conflict(q.R, e_i, low_e))
      return false;

    P.R = merge_edge_ranges(q.R, P.R, ref, side);
    P.L = merge_edge_ranges(q.L, P.L, ref, side);
  }

  if (!P.is_empty())
    S.push_back(P);

  return true;
}

int lowest(EdgeConstraint &P, std::vector<int> &low_e) {
  if (P.L.is_empty())
    return low_e[P.R.low.id];
  if (P.R.is_empty())
    return low_e[P.L.low.id];
  return std::min(low_e[P.L.low.id], low_e[P.R.low.id]);
}

void remove(int f, std::vector<int> &low_e, std::vector<EnumEdge> &ref,
            std::vector<int> &side, std::vector<EdgeConstraint> &S) {
  EdgeConstraint P;
  while (!S.empty() && lowest(S.back(), low_e) == f) {
    P = S.back();
    S.pop_back();

    if (P.L.low.a != -1)
      side[P.L.low.id] = -1;
    if (P.R.low.a != -1)
      side[P.R.low.id] = 1;
  }

  if (!S.empty()) {
    P = S.back();
    S.pop_back();
    // Remove edges from P.L.
    while (!P.L.is_empty() && low_e[P.L.high.id] == f)
      P.L.high = ref[P.L.high.id];
    if (P.L.high.is_empty() && !P.L.low.is_empty()) {
      ref[P.L.low.id] = P.R.low;
      side[P.L.low.id] = -1;
      P.L.low = P.L.high = EnumEdge::empty();
    }

    // Remove edges from P.R.
    while (!P.R.is_empty() && low_e[P.R.high.id] == f)
      P.R.high = ref[P.R.high.id];
    if (P.R.high.is_empty() && !P.R.low.is_empty()) {
      ref[P.R.low.id] = P.L.low;
      side[P.R.low.id] = -1;
      P.R.low = P.R.high = EnumEdge::empty();
    }

    if (!P.is_empty())
      S.push_back(P);
  }
}

bool dfs2(int v, EnumEdge e,
          std::vector<std::vector<std::pair<int, int>>> &elist,
          std::vector<int> &low_e, std::vector<int> &bottom,
          std::vector<EnumEdge> &low_edge, std::vector<EnumEdge> &ref,
          std::vector<int> &side, std::vector<EdgeConstraint> &S) {
  for (auto &[u, id] : elist[v]) {
    EnumEdge e_i(v, u, id);
    bottom[e_i.id] = S.size() - 1;
    if (is_T({v, u})) {
      if (!dfs2(u, e_i, elist, low_e, bottom, low_edge, ref, side, S))
        return false;
    } else {
      low_edge[e_i.id] = e_i;
      S.emplace_back(EnumEdge::empty(), EnumEdge::empty(), e_i, e_i);
    }

    if (low_e[e_i.id] < v) {
      if (u == elist[v][0].first) // e_i is the first edge from v
        low_edge[e.id] = low_edge[e_i.id];
      else if (!merge(e, e_i, bottom[e_i.id], low_e, low_edge, ref, side, S))
        return false;
    }
  }

  int f = e.a;

  if (f != 0) {
    // Remove all edges going to f from S.
    remove(f, low_e, ref, side, S);

    // Set ref[e = {w, v}] to be the edge f = {x, high[e]}. If it exists, it
    // must be on top of S.
    EnumEdge g;

    if (!S.empty()) {
      EdgeConstraint P = S.back();
      g = (P.L.high.b > P.R.high.b) ? P.L.high : P.R.high;
    }
    ref[e.id] = g;
  }

  return true;
}

int eval_sign(int e_id, std::vector<int> &sign, std::vector<EnumEdge> &ref,
              std::vector<int> &side) {
  if (sign[e_id] == 0 && ref[e_id].is_empty())
    sign[e_id] = side[e_id];
  else if (sign[e_id] == 0)
    sign[e_id] = side[e_id] * eval_sign(ref[e_id].id, sign, ref, side);
  return sign[e_id];
}

void dfs3(int v, int f, std::vector<bool> &visited,
          std::vector<std::list<std::pair<int, int>>> &adj,
          std::vector<int> &sign,
          std::vector<std::list<std::pair<int, int>>::iterator> &left,
          std::vector<std::list<std::pair<int, int>>::iterator> &right) {
  visited[v] = true;
  auto it = adj[v].begin();
  while (it != adj[v].end()) {
    std::pair<int, int> e = *it;
    int w = e.first;
    int id = e.second;

    if (!visited[w]) { // tree edge
      adj[w].push_front({v, -1});
      left[v] = right[v] = it;
      dfs3(w, v, visited, adj, sign, left, right);
    } else if (w != f && v > w) { // back edge
      if (sign[id] == 1) {        // right edge
        right[w] = adj[w].insert(right[w], {v, -1});
      } else { // left edge
        adj[w].insert(std::next(left[w]), {v, -1});
      }
    }

    std::advance(it, 1);
  }
}

/** Checks whether graph is planar and returns its combinatoric
 * embedding if it is. Uses Fraysseix-Mendez-Rosenstiehl algorithm. Assumes the
 * graph is connected and has no multiedges or loops.
 *
 * @complexity O(|V|)
 * @param list Adjacency list of the graph
 * @return Adjacency list sorted according to combinatoric embedding if the
 * graph is planar, empty list otherwise.
 */
std::vector<std::list<int>> is_planar(std::vector<std::list<int>> list) {
  int n = list.size() - 1;

  if (n == 1)
    return {{}, {}};
  else if (n == 2)
    return {{}, {2}, {1}};

  int m = 0;
  for (int v = 1; v <= n; v++)
    m += list[v].size();
  m /= 2;

  // Following Euler's formula, |E| <= 3 * |V| - 6 for all planar graphs with
  // |V| >= 3.
  if (m > 3 * n - 6)
    return {};

  std::vector<int> preorder(n + 1, 0);
  std::vector<int> low(n + 1, 0);
  std::vector<int> low2(n + 1, 0);

  int last_preorder = 0;
  std::vector<bool> visited(n + 1, false);
  std::vector<std::pair<int, int>> T;

  dfs1(1, 0, visited, last_preorder, list, preorder, low, low2, T);

  // Remove edges not appearing in DFS orientation. (in other words, orient the
  // graph according to dfs orientation).
  for (int v = 1; v <= n; v++)
    list[v].clear();
  for (auto e : T)
    list[e.first].push_back(e.second);

  // Renumerate adjacency list according to preorder.
  renumerate(list, preorder);

  // Calculate order B on edges.
  std::vector<std::vector<std::pair<int, int>>> B(2 * n + 3);
  for (int v = 1; v <= n; v++) {
    for (auto u : list[v]) {
      if (is_T({v, u}))
        B[(low2[u] >= v) ? 2 * low[u] : 2 * low[u] + 1].push_back({v, u});
      else
        B[2 * u].push_back({v, u});
    }
  }

  // Sort adjacency list according to calculated order B and enhance edges with
  // id.
  std::vector<std::vector<std::pair<int, int>>> elist(n + 1);
  int e_id = 0;
  for (int c = 1; c <= 2 * n + 1; c++)
    for (auto edge : B[c])
      elist[edge.first].emplace_back(std::make_pair(edge.second, e_id++));

  // Renumerate low function indices according to preorder.
  std::vector<int> _low(n + 1);
  std::vector<int> _low2(n + 1);
  for (int v = 1; v <= n; v++) {
    _low[preorder[v]] = low[v];
    _low2[preorder[v]] = low2[v];
  }
  low = _low;
  low2 = _low2;

  // Calculate low function for edges.
  std::vector<int> low_e(e_id);
  for (int v = 1; v <= n; v++)
    for (auto &[u, id] : elist[v])
      low_e[id] = is_T({v, u}) ? low[u] : u;

  std::vector<int> bottom(e_id), side(e_id);
  std::vector<EnumEdge> ref(e_id), low_edge(e_id);
  std::vector<EdgeConstraint> S;

  // All edges are in R by default.
  for (int v = 1; v <= n; v++)
    for (auto &[u, id] : elist[v]) {
      side[id] = 1;
      ref[id] = EnumEdge(-1, -1, -1);
    }

  if (!dfs2(1, EnumEdge(0, 1, -2), elist, low_e, bottom, low_edge, ref, side,
            S))
    return {};

  // Evaluate sign of each edge based on calculation in dfs2.
  std::vector<int> sign(e_id, 0);
  for (int v = 1; v <= n; v++)
    for (auto &[u, id] : elist[v])
      sign[id] = eval_sign(id, sign, ref, side);

  // Find embedding for DFS-orientation edges.
  std::vector<std::list<std::pair<int, int>>> adj(n + 1);
  for (int v = 1; v <= n; v++) {
    for (auto &[u, id] : elist[v])
      if (sign[id] == -1) // left edges
        adj[v].push_back({u, id});
  }
  for (int v = 1; v <= n; v++) {
    for (auto &[u, id] : elist[v])
      if (sign[id] == 1) // right edges
        adj[v].push_front({u, id});
  }

  std::vector<std::list<std::pair<int, int>>::iterator> left(n + 1),
      right(n + 1);
  for (int v = 1; v <= n; v++)
    visited[v] = false;

  dfs3(1, 0, visited, adj, sign, left, right);

  // Generate return list from adj.
  for (int v = 1; v <= n; v++) {
    list[v].clear();
    for (auto &[u, id] : adj[v])
      list[v].push_back(u);
  }

  return list;
}

/** Finds planar graphs faces. Assumes the graph has no loops or
 * multiedges.
 *
 *  @complexity O(|V|)
 *  @param list Enumerated adjacency list of graph (combinatorial embedding).
 *  @param CR Graph edges cross references.
 *  @return vector of graph faces.
 */
std::vector<std::vector<std::pair<int, int>>>
find_faces(std::vector<std::list<std::pair<int, int>>> &list,
           std::vector<EdgeCrossRef> &CR) {
  int n = list.size() - 1;
  int m = CR.size() - 1;
  std::vector<bool> visited(
      2 * m, false); // tracking which directed edges have been used. Note
                     // that directed edges have ids from [0..2m-1].

  /** Get directed edge u->v id, where id param is the undirected edge id. */
  auto dir_id = [&](int u, int v, int id) {
    return (u < v) ? 2 * (id - 1) : 2 * (id - 1) + 1;
  };

  std::vector<std::vector<std::pair<int, int>>> faces;

  for (int u = 1; u <= n; u++) {
    for (auto &[v, id] : list[u]) {
      if (!visited[dir_id(u, v, id)]) {
        faces.push_back({{u, id}});
        visited[dir_id(u, v, id)] = true;
        int x = u;
        int y = v;
        int e_id = id;
        while (true) {
          auto it = CR[e_id].getCrossRef(x);

          it = (it == list[y].begin()) ? std::prev(list[y].end())
                                       : std::prev(it);

          auto [z, next_e_id] = *it;

          if (visited[dir_id(y, z, next_e_id)])
            break;

          visited[dir_id(y, z, next_e_id)] = true;
          faces.back().push_back({y, next_e_id});

          x = y;
          y = z;
          e_id = next_e_id;
        }
      }
    }
  }

  return faces;
}
