#include "embedding.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

std::vector<std::list<int>> input(std::ifstream &input_file) {
  int n, m;
  input_file >> n >> m;
  std::vector<std::list<int>> list(n + 1);
  int u, v;
  for (int i = 0; i < m; i++) {
    input_file >> u >> v;
    list[u].push_back(v);
    list[v].push_back(u);
  }

  return list;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: aracli [input file path].\n";
    return 1;
  }

  std::ifstream input_file;
  input_file.open(argv[1]);

  std::vector<std::list<int>> list = input(input_file);

  std::vector<std::pair<int, int>> embedding = planar_embedding(list);

  embedding.erase(embedding.begin());

  for (auto &[x, y] : embedding)
    std::cout << x << " " << y << "\n";
}
