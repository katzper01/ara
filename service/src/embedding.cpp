#include <assert.h>
#include <list>
#include <stdexcept>
#include <vector>

#include "planar.hpp"
#include "triangulation.hpp"
#include "utils.hpp"

// DFS for calculating p[] and t[] values in T_i.
void dfs_pt(int v, int i,
            const std::vector<std::list<std::pair<int, int>>> &elist,
            const std::vector<std::pair<int, int>> &SW,
            std::vector<std::vector<int>> &p,
            std::vector<std::vector<int>> &t) {
  t[i][v] = 1;
  for (auto &[u, id] : elist[v]) {
    auto [d, color] = SW[id];
    bool isDirectedToV = (dir(u, v) == d);
    if (color == i && isDirectedToV) {
      p[i][u] = p[i][v] + 1;
      dfs_pt(u, i, elist, SW, p, t);
      t[i][v] += t[i][u];
    }
  }
}

// Dfs for calculating r[] values in T_i. ST = \sum t[v] on path from root.
void dfs_r(int v, int i,
           const std::vector<std::list<std::pair<int, int>>> &elist,
           const std::vector<std::pair<int, int>> &SW,
           std::vector<std::vector<int>> &t, std::vector<std::vector<int>> &r,
           std::vector<int> ST) {
  for (int j = 0; j < 3; j++) {
    ST[j] += t[j][v];
    if (j != i)
      r[j][v] += ST[j];
  }

  for (auto &[u, id] : elist[v]) {
    auto [d, color] = SW[id];
    bool isDirectedToV = (dir(u, v) == d);
    if (color == i && isDirectedToV) {
      dfs_r(u, i, elist, SW, t, r, ST);
    }
  }
}

/**
 * Calculates several values for planar triangulation with Schnyder wood T_0,
T_1, T_2:
 *     - p[i][v] - number of vertices on path from v to root of T_i in T_i,
 *     - t[i][v] - size of subtree with root v in T_i,
 *     - r[i][v] - number of vertices in region bounded by path
 *                 v->root(T_{i+1}), path v->root(T_{i-1}) and
 *                 edge root(T_{i-1})<-->root(T_{i+1}).
 *     - w[i][v] = r[i][w] - p[i-1][w]
 * Turns out that (w[0][v], w[1][v]) is planar straight-line embedding.
 */
std::vector<std::pair<int, int>> find_embedding_for_schnyder_wood(
    const std::vector<std::list<std::pair<int, int>>> &elist,
    const std::vector<std::pair<int, int>> &SW, int a, int b, int c) {
  int n = elist.size() - 1;

  std::vector<std::vector<int>> p(3, std::vector<int>(n + 1, 0));
  std::vector<std::vector<int>> t(3, std::vector<int>(n + 1, 0));

  // Define t[i][v] for outer vertices v not in T_i.
  t[0][b] = t[1][a] = t[2][c] = 1;

  // Define p[i][v] for roots of T_i.
  p[0][c] = p[1][b] = p[2][a] = 1;

  // Calculate p[] and t[] values.
  dfs_pt(c, 0, elist, SW, p, t);
  dfs_pt(b, 1, elist, SW, p, t);
  dfs_pt(a, 2, elist, SW, p, t);

  // Calculate r[] values.
  std::vector<std::vector<int>> r(3, std::vector<int>(n + 1, 0));
  dfs_r(c, 0, elist, SW, t, r, {0, 0, 0});
  dfs_r(b, 1, elist, SW, t, r, {0, 0, 0});
  dfs_r(a, 2, elist, SW, t, r, {0, 0, 0});

  for (int i = 0; i < 3; i++)
    for (int v = 1; v <= n; v++)
      r[i][v] -= t[i][v];

  // Calculate w[] values.
  std::vector<std::vector<int>> w(3, std::vector<int>(n + 1, 0));

  for (int i = 0; i < 3; i++)
    for (int v = 1; v <= n; v++)
      w[i][v] = r[i][v] - p[(i + 2) % 3][v];

  // For outer vertices w[] are different.
  w[0][c] = n - 2;
  w[1][c] = 1;
  w[1][b] = n - 2;
  w[2][b] = 1;
  w[2][a] = n - 1;
  w[0][a] = 1;

  // Build output.
  std::vector<std::pair<int, int>> embedding(n + 1, {-1, -1});

  for (int v = 1; v <= n; v++)
    embedding[v] = {w[0][v], w[1][v]};

  return embedding;
}

/**
 * Finds straight-line planar embedding of a simple graph on
 * {0..|V|-2} x {0..|V|-2} grid. Assumes the graph does not
 * contain loops or multiedges. The graphs does not have to be connected.
 *
 * Throws exception if the graphs is not planar.
 *
 * @complexity O(|V| + |E|)
 * @param list Adjacency list of the graph.
 * @return Planar embedding as vector of coordinates on the grid.
 */
std::vector<std::pair<int, int>>
planar_embedding(std::vector<std::list<int>> &list) {
  list = is_planar(list);

  if (list.empty())
    throw std::invalid_argument("The graph is not planar.");

  std::vector<std::list<int>> triangulation_list = get_triangulation(list);

  std::vector<std::list<std::pair<int, int>>> triangulation_elist =
      enumerate_adjacency_list(triangulation_list);

  std::vector<EdgeCrossRef> CR = getEdgeCrossReferences(triangulation_elist);

  std::vector<std::vector<std::pair<int, int>>> faces =
      find_faces(triangulation_elist, CR);

  assert(!faces.empty() && faces[0].size() == 3);

  int a = faces[0][0].first;
  int b = faces[0][1].first;
  int c = faces[0][2].first;

  std::vector<std::pair<int, int>> SW =
      get_schnyder_wood(triangulation_elist, a, b, c);

  return find_embedding_for_schnyder_wood(triangulation_elist, SW, a, b, c);
}
