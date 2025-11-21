#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using label = int;

class UnionFind {
private:
  std::unordered_map<label, label> parents;
  std::unordered_map<label, int> sizes;

  label findParent(label x) {
    if (parents[x] != x) {
      parents[x] = findParent(parents[x]);
    }
    return parents[x];
  }

public:
  UnionFind(const std::vector<label> universum) {
    for (const auto item : universum) {
      parents[item] = item;
      sizes[item] = 1;
    }
  }

  bool isInSameComponent(label x, label y) {
    return findParent(x) == findParent(y);
  }

  void uniteComponents(label x, label y) {
    label parentX = findParent(x);
    label parentY = findParent(y);

    if (parentX == parentY) {
      return;
    }

    if (sizes[parentX] < sizes[parentY]) {
      std::swap(parentX, parentY);
    }

    parents[parentY] = parentX;
    sizes[parentX] += sizes[parentY];
  }
};

struct Edge {
  int weight;
  label from;
  label to;

  bool operator<(const Edge other) { return weight < other.weight; }
};

std::vector<std::vector<int>> readGraphFromFile(const std::string &filename) {
  std::ifstream file(filename);
  std::vector<std::vector<int>> graph;

  if (!file.is_open()) {
    std::cerr << "Unable to open file: " << filename << std::endl;
    return graph;
  }

  std::string line;
  while (getline(file, line)) {
    std::stringstream lineAsStream(line);
    std::vector<int> row;
    int num;

    while (lineAsStream >> num) {
      row.push_back(num);
    }

    if (!row.empty()) {
      graph.push_back(row);
    }
  }

  file.close();
  return graph;
}

int kruskal(const std::vector<std::vector<int>> &graph) {
  int n = graph.size();

  std::vector<label> verticies(n);
  for (int i = 0; i < n; i++) {
    verticies[i] = i + 1;
  }

  std::vector<Edge> edges;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (graph[i][j] != 0) {
        edges.push_back({graph[i][j], i + 1, j + 1});
      }
    }
  }

  std::sort(edges.begin(), edges.end());

  UnionFind uf(verticies);
  int edge_counter = 0;
  int k = 0;
  int total_weight = 0;

  while (edge_counter < n - 1) {
    const Edge &e = edges[k];
    k++;

    if (!uf.isInSameComponent(e.from, e.to)) {
      uf.uniteComponents(e.from, e.to);
      total_weight += e.weight;
      edge_counter++;
    }
  }
  return total_weight;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  auto graph = readGraphFromFile(argv[1]);
  int result = kruskal(graph);
  std::cout << result << std::endl;

  return 0;
}
