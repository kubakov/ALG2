#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

struct SimplePairHash {
  size_t operator()(const std::pair<int, int> &inPair) const {
    return inPair.first + 10000 * inPair.second;
  }
};

std::string readTextFromFile(const std::string &fileName) {
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "Error: Unable to open file " << fileName << std::endl;
    exit(1);
  }
  std::string text((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  return text;
}

std::string escapeSpecialChars(const std::string &s) {
  std::string result;
  for (char c : s) {
    if (c == '\n') {
      result += "\\n";
    } else if (c == '\t') {
      result += "\\t";
    } else {
      result += c;
    }
  }
  return result;
}

void bytePairEncoding(const std::string &text, int mapSize) {
  std::unordered_map<std::string, int> stringToCode;
  std::unordered_map<int, std::string> codeToString;

  std::vector<int> textAsNumbers;

  int currCode = 0;

  for (char c : text) {
    std::string s(1, c);
    if (!stringToCode.count(s)) {
      stringToCode[s] = currCode;
      codeToString[currCode] = s;
      currCode++;
    }
    textAsNumbers.push_back(stringToCode[s]);
  }

  if (currCode >= mapSize) {
    for (int i = 0; i < currCode; i++) {
      std::cout << i << " -> '" << escapeSpecialChars(codeToString.at(i))
                << "'\n";
    }
    return;
  }

  while ((int)stringToCode.size() < mapSize) {
    std::unordered_map<std::pair<int, int>, int, SimplePairHash> pairFrequency;

    for (size_t i = 0; i + 1 < textAsNumbers.size(); i++) {
      std::pair<int, int> p = {textAsNumbers[i], textAsNumbers[i + 1]};
      pairFrequency[p]++;
    }

    if (pairFrequency.empty()) {
      break;
    }

    int mostFreq = -1;
    std::pair<int, int> mostFreqPair = {-1, -1};

    for (const auto &kv : pairFrequency) {
      const auto &p = kv.first;
      int count = kv.second;

      if (count > mostFreq || (count == mostFreq && p > mostFreqPair)) {
        mostFreq = count;
        mostFreqPair = p;
      }
    }

    int newCode = currCode++;
    std::string paired =
        codeToString[mostFreqPair.first] + codeToString[mostFreqPair.second];

    stringToCode[paired] = newCode;
    codeToString[newCode] = paired;

    std::vector<int> newTextAsNumbers;
    for (size_t i = 0; i < textAsNumbers.size();) {
      if (i + 1 < textAsNumbers.size() &&
          textAsNumbers[i] == mostFreqPair.first &&
          textAsNumbers[i + 1] == mostFreqPair.second) {
        newTextAsNumbers.push_back(newCode);
        i += 2;
      } else {
        newTextAsNumbers.push_back(textAsNumbers[i]);
        i += 1;
      }
    }
    textAsNumbers.swap(newTextAsNumbers);
  }
  for (int i = 0; i < (int)codeToString.size(); i++) {
    std::cout << i << " -> '" << escapeSpecialChars(codeToString[i]) << "'\n";
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  std::string file = argv[1];
  int mapSize = std::stoi(argv[2]);

  std::string text = readTextFromFile(file);

  bytePairEncoding(text, mapSize);
}
