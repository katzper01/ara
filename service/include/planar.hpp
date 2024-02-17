#pragma once

#include "utils.hpp"
#include <list>
#include <vector>

/** Checks whether graph is planar and returns its combinatoric
 * embedding if it is. Uses Fraysseix-Mendez-Rosenstiehl algorithm. Assumes the
 * graph is connected and has no multiedges or loops.
 *
 * @complexity O(|V|)
 * @param list Adjacency list of the graph
 * @return Adjacency list sorted according to combinatoric embedding if the
 * graph is planar, empty list otherwise.
 */
std::vector<std::list<int>> is_planar(std::vector<std::list<int>> list);

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
           std::vector<EdgeCrossRef> &CR);
