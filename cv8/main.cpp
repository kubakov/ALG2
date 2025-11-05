#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <algorithm>

using std::string, std::vector;

class naiveHashTable {
private:
	vector<vector<string>> table;
	size_t numberOfKeys;
	double maxOccupancy;
	double resizeFactor;

	void resize() {
		size_t newSize = static_cast<size_t>(table.size() * resizeFactor);
		vector<vector<string>> newTable(newSize);

		for (const auto& bucket : table) {
			for (const auto& key : bucket) {
				size_t index = hashToIndex(key, table.size());
				table[index].push_back(key);
			}
		}

		table = std::move(newTable);
	}

	bool resizeNeed() const {
		return static_cast<double>(numberOfKeys) / table.size() > maxOccupancy;
	}

	size_t hashToIndex(const string& key, size_t size) {
		return std::hash<string>{}(key) % size;
	}

public:

	bool lookup(const string& key) {
		size_t index = hashToIndex(key, table.size());
		return std::find(table[index].begin(), table[index].end(), key) != table[index].end();
	}

	bool insert(const string& key) {
		if (lookup(key)) { return false; }
		numberOfKeys += 1;
		if (resizeNeed()) {
			resize();
		}

		size_t index = hashToIndex(key, table.size());
		table[index].push_back(key);
		return true;
	}

	bool remove(const string& key) {
		if (lookup(key)) { return false; }

		numberOfKeys -= 1;
		size_t index = hashToIndex(key, table.size());
		
		auto key_it = std::find(table[index].begin(), table[index].end(), key);
		std::swap(table[index].back(), *key_it);
		table[index].pop_back();
		return true;
	}
};

enum State {
	EMPTY,
	DELETED,
	OCCUPIED
};

struct Entry
{
	string key;
	State state;
};

class linearProbingTable {
private:
	vector<Entry> table;
	size_t numberOfKeys;
	double maxOccupancy;
	double resizeFactor;

	bool resizeNeed() const {
		return static_cast<double>(numberOfKeys) / table.size() > maxOccupancy;
	}

	void resize() {
		size_t newSize = static_cast<size_t>(resizeFactor * table.size());

		vector<Entry> oldTable = std::move(table);
		table.assign(newSize, Entry({ "", EMPTY }));

		for (size_t i = 0; i < oldTable.size(); i++) {//pro stejný výstup na test v úkolu
			if (oldTable[i].state == OCCUPIED) {
				insert(oldTable[i].key);
			}
		}
	}

	size_t hashToIndex(const string& key, size_t size) {
		return std::hash<string>{}(key) % size;
	}

public:
	bool lookup(const string& key) {
		size_t initialIndex = hashToIndex(key, table.size());
		for (size_t i = 0; i < table.size(); i++) {
			size_t currentIndex = (initialIndex + i) % table.size();
			const Entry& entry = table[currentIndex];

			if (entry.state == EMPTY) {
				return false;
			}

			if (entry.state == OCCUPIED && entry.key == key) {
				return true;
			}
			return false;
		}
	}

	bool insert(const string& key) {
		if (resizeNeed()) {
			resize();
		}

		std::optional<size_t> insertIndex = std::nullopt;
		size_t initialIndex = hashToIndex(key, table.size());

		for (size_t i = 0; i < table.size(); i++) {
			size_t currentIndex = (initialIndex + i) % table.size();
			Entry& entry = table[currentIndex];

			if (entry.state == OCCUPIED and key == entry.key) {
				return false;
			}

			if (entry.state == EMPTY) {
				if (!insertIndex.has_value()) {
					insertIndex = currentIndex;
				}

				table[insertIndex.value()].state = OCCUPIED;
				table[insertIndex.value()].key = key;
				numberOfKeys += 1;
				return true;
			}

			if (entry.state == DELETED) {
				if (!insertIndex.has_value()) {
					insertIndex = currentIndex;
				}
			}
		}
		if (insertIndex.has_value()) {
			table[insertIndex.value()].state = OCCUPIED;
			table[insertIndex.value()].key = key;
			numberOfKeys += 1;
			return true;
		}
		return false;
	}

	bool remove(const string& key) {
		size_t initialIndex = hashToIndex(key, table.size());
		for (size_t i = 0; i < table.size(); i++) {
			size_t currentIndex = (initialIndex + i) % table.size();
			Entry& entry = table[currentIndex];

			if (entry.state == EMPTY) {
				return false;
			}

			if (entry.state == OCCUPIED && entry.key == key) {
				entry.state = DELETED;
				entry.key = "";
				numberOfKeys -= 1;
				return true;
			}
			return false;
		}
	}
};

int main() {
			
	return 0;
}