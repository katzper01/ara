#include "embedding.hpp"
#include <iostream>
#include <list>
#include <vector>

std::vector<std::list<int>> input() {
  int n, m;
  std::cin >> n >> m;
  std::vector<std::list<int>> list(n + 1);
  int u, v;
  for (int i = 0; i < m; i++) {
    std::cin >> u >> v;
    list[u].push_back(v);
    list[v].push_back(u);
  }

  return list;
}

int main() {
  std::vector<std::list<int>> list = input();

  std::vector<std::pair<int, int>> embedding = planar_embedding(list);

  embedding.erase(embedding.begin());

  for (auto &[x, y] : embedding)
    std::cout << x << " " << y << "\n";
}
