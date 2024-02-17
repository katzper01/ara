#include "planar.hpp"
#include "utils.hpp"
#include <algorithm>
#include <list>
#include <queue>
#include <vector>

/**
 * Makes planar graph connected and calculates its new combinational embedding.
 * Since given combinational embedding, every face can be drawn as external,
 * this is a very simple procedure.
 */
void make_connected(std::vector<std::list<int>> &list) {
  int n = list.size() - 1;
  std::vector<bool> visited(n + 1, false);
  std::vector<int> component_root;
  std::queue<int> q;
  for (int v = 1; v <= n; v++) {
    if (!visited[v]) {
      component_root.push_back(v);
      q.push(v);
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto w : list[u])
          if (!visited[w]) {
            visited[w] = true;
            q.push(w);
          }
      }
    }
  }

  for (int i = 0; i < (int)component_root.size() - 1; i++) {
    int u = component_root[i];
    int v = component_root[i + 1];
    list[u].push_back(v);
    list[v].push_back(u);
  }
}

/**
 * Makes connected planar graph 2-connected. It does so by forming cycles
 * consisting of each articulation point's neighbors.
 */
void make_2connected(std::vector<std::list<std::pair<int, int>>> &list,
                     std::vector<EdgeCrossRef> &CR) {
  GraphConnectivity GC(list);
  std::vector<int> bcid = GC.bcid;
  std::vector<int> art = GC.getArticulationPoints();
  std::reverse(art.begin(), art.end());

  for (auto v : art) {
    int new_edges_bcid = (GC.p_edge_id[v] == -1) ? 0 : bcid[GC.p_edge_id[v]];

    for (auto it = list[v].begin(); it != list[v].end(); ++it) {
      auto it1 = std::next(it);
      if (it1 == list[v].end()) {
        if (list[v].size() > 2)
          it1 = list[v].begin();
        else
          break;
      }
      auto [u1, e1] = *it;
      auto [u2, e2] = *it1;

      if (bcid[e1] != bcid[e2]) { // if e1 and e2 are not in the same
                                  // biconnected comp, add edge u1-u2.
        int new_edge_id = CR.size();
        auto e1p_it = CR[e1].getCrossRef(v);
        auto new_edge_it1 = list[u1].insert(e1p_it, {u2, new_edge_id});
        auto e2p_it = CR[e2].getCrossRef(v);
        auto new_edge_it2 =
            list[u2].insert(std::next(e2p_it), {u1, new_edge_id});
        CR.emplace_back(new_edge_it1, new_edge_it2);
        bcid.push_back(new_edges_bcid);
      }
    }
  }
}

/** Triangulates given faces. Assumes graph is biconnected. */
void triangulate_faces(std::vector<std::list<std::pair<int, int>>> &list,
                       std::vector<EdgeCrossRef> &CR,
                       std::vector<std::vector<std::pair<int, int>>> &faces) {
  int n = list.size() - 1;
  std::vector<bool> nx(n + 1, false);
  for (auto &face : faces) {
    int t = face.size();

    // find vertex in face with minimum degree.
    auto [x, _] = face[0];
    int pos_x = 0;
    for (int i = 1; i < t; i++) {
      int v = face[i].first;
      if (list[v].size() < list[x].size()) {
        x = v;
        pos_x = i;
      }
    }

    // rotate face to make x the first element (for convenience).
    std::rotate(face.begin(), face.begin() + pos_x, face.end());

    // set neighbours of x.
    for (auto &[v, _] : list[x])
      nx[v] = true;

    // triangulate as much as we can from x and let y be the last for which we
    // succeeded.
    auto [y, e1_id] = face[1];  // e1 = (y, z) at all times
    int e2_id = face[0].second; // e2 = (x, y) at all times

    int i = 2;
    while (i <= t - 2) {
      int z = face[i].first;
      if (nx[z])
        break;

      // add edge e3 = (x, z)
      int e3_id = CR.size();
      auto e2_x_it = CR[e2_id].getCrossRef(y);
      auto e3_x_it = list[x].insert(std::next(e2_x_it), {z, e3_id});
      auto e1_z_it = CR[e1_id].getCrossRef(y);
      auto e3_z_it = list[z].insert(e1_z_it, {x, e3_id});
      CR.emplace_back(e3_x_it, e3_z_it);

      e1_id = face[i].second;
      e2_id = e3_id;
      y = z;
      i++;
    }

    // unset neighbours of x.
    for (auto &[v, _] : list[x])
      nx[v] = false;

    // if there is something left in the face, triangulate from y.
    if (i == t - 1)
      continue;

    x = y;
    y = face[i].first;
    e1_id = face[i].second;
    e2_id = face[i - 1].second;
    i++;

    while (i <= t - 1) {
      int z = face[i].first;

      // add edge e3 = (x, z)
      int e3_id = CR.size();
      auto e2_x_it = CR[e2_id].getCrossRef(y);
      auto e3_x_it = list[x].insert(std::next(e2_x_it), {z, e3_id});
      auto e1_z_it = CR[e1_id].getCrossRef(y);
      auto e3_z_it = list[z].insert(e1_z_it, {x, e3_id});
      CR.emplace_back(e3_x_it, e3_z_it);

      e1_id = face[i].second;
      e2_id = e3_id;
      y = z;
      i++;
    }
  }
}

/**
 * Returns combinational embedding of given planar graphs triangulation.
 * Does not assume the graph is connected. Assumes the provided adjacency list
 * corresponds to a planar combinational embedding of the graph and that the
 * graph has at least 3 vertices and is loop-free and multiedge-free. The
 * algorithm first makes the graph connected, then 2-connected and at the end,
 * triangulates all faces.
 *
 * @complexity O(|V|)
 * @param list Adjacency list of the graph (sorted according to combinational
 * embedding)
 * @return Adjacency list of graph triangulation
 */
std::vector<std::list<int>>
get_triangulation(std::vector<std::list<int>> list) {
  int n = list.size() - 1;
  int m = 0;
  for (auto &l : list)
    m += l.size();
  m /= 2;

  make_connected(list);

  std::vector<std::list<std::pair<int, int>>> elist =
      enumerate_adjacency_list(list);

  std::vector<EdgeCrossRef> CR = getEdgeCrossReferences(elist);

  make_2connected(elist, CR);

  std::vector<std::vector<std::pair<int, int>>> faces = find_faces(elist, CR);

  triangulate_faces(elist, CR, faces);

  // Build output.
  for (int v = 1; v <= n; v++) {
    list[v].clear();
    for (auto &[u, id] : elist[v])
      list[v].push_back(u);
  }

  return list;
}

/**
 * Returns Schnyder wood for given planar triangulation graph and set outer
 * face f = {a, b, c}. c will be 0-root, a will be 2-root, b will be 1-root.
 *
 * @complexity O(|V|)
 * @param list Adjacency list of given graph. Assumes edges v-u in list[v] are
 * represented by pairs {u, id}, where id \in [1..m]
 * @return vector v such that v[edge_id] = {dir, color} and:
 *     - dir = -1 if the edge is directed towards vertex with smaller id, 1
 * otherwise,
 *     - color \in {0, 1, 2} represents the edge color.
 */
std::vector<std::pair<int, int>>
get_schnyder_wood(std::vector<std::list<std::pair<int, int>>> &list, int a,
                  int b, int c) {
  int n = list.size() - 1;
  int m = 0;
  for (auto &l : list)
    m += l.size();
  m /= 2;

  std::vector<std::pair<int, int>> SW(m + 1, {-1, -1});
  std::vector<bool> on_cycle(n + 1, false);
  std::vector<int> cycle_nodes(n + 1, 0);
  std::vector<bool> processed(n + 1, false);
  std::queue<int> good_vertices;

  auto is_good = [&](int v) {
    bool is_interior = (v != a && v != b && v != c);
    return is_interior && !processed[v] && on_cycle[v] && cycle_nodes[v] == 2;
  };

  auto check = [&](int v) {
    if (is_good(v))
      good_vertices.push(v);
  };

  auto add_to_cycle = [&](int v) {
    on_cycle[v] = true;
    for (auto &[u, id] : list[v]) {
      cycle_nodes[u]++;
      check(u);
    }
    check(v);
  };

  for (auto &[u, e_id] : list[c]) {
    add_to_cycle(u);
    if (u != a && u != b)
      SW[e_id] = {dir(u, c), 0};
  }

  processed[c] = true;

  /** Cyclic next in list. */
  auto cnext = [](std::list<std::pair<int, int>> &l,
                  typename std::list<std::pair<int, int>>::iterator &it) {
    std::advance(it, 1);
    if (it == l.end())
      it = l.begin();
  };

  // Main loop.
  while (!good_vertices.empty()) {
    int v = good_vertices.front();
    good_vertices.pop();

    if (!is_good(v))
      continue;

    // Every good vertex has an already processed neighbour.
    auto it = list[v].begin();
    while (!processed[it->first])
      ++it;

    // Process cycle neighbour of v "to the right".
    while (!on_cycle[it->first])
      cnext(list[v], it);

    cycle_nodes[it->first]--;
    SW[it->second] = {dir(v, it->first), 1};

    check(it->first);

    cnext(list[v], it);

    // Process all neighbours of v inside cycle.
    while (!on_cycle[it->first]) {
      cycle_nodes[it->first]--;
      SW[it->second] = {dir(it->first, v), 0};
      add_to_cycle(it->first);

      cnext(list[v], it);
    }

    // Process cycle neighbour of v "to the left".
    cycle_nodes[it->first]--;
    SW[it->second] = {dir(v, it->first), 2};
    check(it->first);

    on_cycle[v] = false;
    processed[v] = true;
  }

  // The remaining edges are between a, b and c.
  for (auto &[u, id] : list[a])
    if (u == c)
      SW[id] = {dir(a, c), 0};
  for (auto &[u, id] : list[c])
    if (u == b)
      SW[id] = {dir(c, b), 1};
  for (auto &[u, id] : list[b])
    if (u == a)
      SW[id] = {dir(b, a), 2};

  return SW;
}
