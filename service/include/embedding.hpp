#pragma once

#include <list>
#include <vector>

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
planar_embedding(std::vector<std::list<int>> &list);
