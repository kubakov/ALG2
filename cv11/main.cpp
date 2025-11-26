#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <optional>
#include <limits>

using std::vector, std::priority_queue;

struct Node {
	double frequency;
	Node* left;
	Node* right;
	bool isLeaf;
	std::optional<char> character;

	Node(double f, char ch) : frequency(f), left(nullptr), right(nullptr), isLeaf(true), character(ch) {}

	Node(double f) : frequency(f), left(nullptr), right(nullptr), isLeaf(false), character(std::nullopt) {}
};

struct CompareNode {
	bool operator()(const Node*& node1, const Node*& node2) {
		return node1->frequency < node2->frequency;
	}
};

Node* buildHuffmanTree(const std::unordered_map<char, double> frequencies) {
	priority_queue<Node*, vector<Node*>, CompareNode> minHeap;

	for (const auto& [ch, f] : frequencies) {
		minHeap.push(new Node(f, ch));
	}

	while (minHeap.size() > 1) {
		Node* left = minHeap.top(); minHeap.pop();
		Node* right = minHeap.top(); minHeap.pop();

		Node* merged = new Node(left->frequency + right->frequency);
		merged->left = left;
		merged->right = right;

		minHeap.push(merged);
	}

	return minHeap.top();
}

void buildTable(Node* node, std::string& prefix, std::unordered_map<char, std::string>& table) {
	if (node->isLeaf) {
		table[node->character.value()] = prefix;
	}
	if (node->left != nullptr) {
		std::string tmp = prefix;
		tmp += "0";
		buildTable(node->left, tmp, table);
	}
	if (node->right != nullptr) {
		std::string tmp = prefix;
		tmp += "1";
		buildTable(node->right, tmp, table);
	}
}

using label = int;
using weightedEdge = std::pair<int, label>;
using Graph = vector<vector<weightedEdge>>;

std::pair<vector<label>, int> dijkstra(const Graph& g, label start, label end) {
	vector<label> path;
	int length = -1;
	const int INF = std::numeric_limits<int>::max();
	vector<int> distances(g.size(), INF);
	vector<label> prev(g.size(), -1);
	priority_queue<weightedEdge, vector<weightedEdge>, std::greater<weightedEdge>> pqDijkstra;
	
	distances[start] = 0;
	pqDijkstra.push({ 0, start });

	while (!pqDijkstra.empty()) {
		auto [currentWeight, currentNode] = pqDijkstra.top();
		pqDijkstra.pop();

		if (currentNode == end) {
			break;
		}

		if (currentWeight > distances[currentNode]) {
			continue;
		}

		for (const auto& [neighbourW, neighbourNode] : g[currentNode]) {
			int newDist = distances[currentNode] + neighbourW;
			if (newDist < distances[neighbourW]) {
				distances[neighbourNode] = newDist;
				prev[neighbourW] = currentNode;
				pqDijkstra.push({ newDist, neighbourW });
			}
		}
	}

	label currentNode = end;
	while (currentNode != start) {
		path.push_back(currentNode);
		currentNode = prev[currentNode];
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());

	length = distances[end];

	return { path, length };
}



int main() {
	
	return 0;
}