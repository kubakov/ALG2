#include <iostream>
#include <vector>
#include <unordered_map>

using label = int;

class UnionFind {
private:
	std::unordered_map<label, label> parents;
	std::unordered_map<label, int> sizes;

	label findParent(label x) {
		if (parents[x] != x) {
			parents[x] = findParent(parents[x]);
		}
		return x;
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

//using Edge = std::pair<label, label>;
//using weightedEdge = std::pair<int, Edge>;

struct Edge {
	int weight;
	label from;
	label to;

	bool operator<(const Edge other) {
		return weight < other.weight;
	}
};

int main() {
	Edge e1({ 2,1,2 });
	Edge e2({ 3,1,3 });
	std::cout << (e2 < e1) << "\n";
	return 0;
}