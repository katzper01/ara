#pragma once

#include <list>
#include <vector>

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
std::vector<std::list<int>> get_triangulation(std::vector<std::list<int>> list);

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
                  int b, int c);
