#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <utility>
#include <algorithm>

using std::string, std::vector, std::optional;

class cuckooTable {
private:
    vector<optional<string>> table1;
    vector<optional<string>> table2;
    size_t numberOfKeys;
    double maxOccupancy;
    double resizeFactor;
    size_t maxLoops;
    
    size_t h1(const string& key) const {
        return std::hash<string>{}(key) % table1.size();
    }
    
    size_t h2(const string& key) const {
        return std::hash<string>{}(key) % table2.size();
    }
    
    void hashResize() {
        vector<string> allKeys;
        allKeys.reserve(numberOfKeys);
        
        for(const auto& opt : table1){
            if(opt.has_value()) {
                allKeys.push_back(opt.value());
            }
        }
        
        for(const auto& opt : table2){
            if(opt.has_value()) {
                allKeys.push_back(opt.value());
            }
        }
        
        size_t newSize1 = static_cast<size_t>(table1.size() * resizeFactor);
        size_t newSize2 = static_cast<size_t>(table2.size() * resizeFactor);
        
        table1.assign(newSize1, std::nullopt);
        table2.assign(newSize2, std::nullopt);
        
        numberOfKeys = 0;
        
        for(const auto& key : allKeys){
            insert(key);
        }
    }
    
public:
    cuckooTable(size_t size1, size_t size2, double occupancy, double factor, size_t loops) {
        numberOfKeys = 0;
        maxOccupancy = occupancy;
        resizeFactor = factor;
        maxLoops = loops;
        table1.resize(size1);
        table2.resize(size2);
    }
    
    bool lookup(const string& key) const {
        size_t idx1 = h1(key);
        if(table1[idx1].has_value() && table1[idx1].value() == key) {
            return true;
        }
    
        size_t idx2 = h2(key);
        if(table2[idx2].has_value() && table2[idx2].value() == key) {
            return true;
        }
        
        return false;
    }
    
    void insert(const string& key) {
        if(lookup(key)){
            return;
        }
        
        if((static_cast<double>(numberOfKeys + 1) / (table1.size() + table2.size())) > maxOccupancy) {
            hashResize();
        }
        
        string currentKey = key;
        for(size_t i = 0; i < maxLoops; i++) {
            size_t idx1 = h1(currentKey);
            if(!table1[idx1].has_value()) {
                table1[idx1] = currentKey;
                numberOfKeys++;
                return;
            }
            std::swap(currentKey, table1[idx1].value());
            
            size_t idx2 = h2(currentKey);
            if(!table2[idx2].has_value()) {
                table2[idx2] = currentKey;
                numberOfKeys++;
                return;
            }
            std::swap(currentKey, table2[idx2].value());
        }
        
        hashResize();
        insert(currentKey);
    }
    
    bool remove(const string& key) {
        size_t idx1 = h1(key);
        if(table1[idx1].has_value() && table1[idx1].value() == key) {
            table1[idx1] = std::nullopt;
            numberOfKeys--;
            return true;
        }
        
        size_t idx2 = h2(key);
        if(table2[idx2].has_value() && table2[idx2].value() == key) {
            table2[idx2] = std::nullopt;
            numberOfKeys--;
            return true;
        }
        
        return false;
    }
    
    void print() const {
        std::cout << "Table1:" << std::endl;
        for (size_t i = 0; i < table1.size(); ++i) {
            if (table1[i].has_value()) {
                std::cout << i << ": " << table1[i].value() << " ";
            }
        }
        std::cout << std::endl;

        std::cout << "Table2:" << std::endl;
        for (size_t i = 0; i < table2.size(); ++i) {
            if (table2[i].has_value()) {
                std::cout << i << ": " << table2[i].value() << " ";
            }
        }
    }
};

int main() {
    cuckooTable table(10, 11, 0.5, 1.5, 5);
    const auto inputs = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    for (const auto& s : inputs) {
        table.insert(s);
    }
    table.print();
    std::cout << std::endl << std::endl;
    
    const auto inputs2 = { "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
                           "seventeen", "eighteen", "nineteen", "twenty", "twenty-one", "twenty-two",
                           "twenty-three", "twenty-four", "twenty-five", "twenty-six", "twenty-seven",
                           "twenty-eight", "twenty-nine"};
    for (const auto& s : inputs2) {
        table.insert(s);
    }
    table.print();
    std::cout << std::endl << std::endl;
    
    const auto removals = { "twenty-one", "twenty-two", "twenty-three", "twenty-four", "twenty-five",
                           "twenty-six", "twenty-seven", "twenty-eight", "twenty-nine",
                           "aardvark", "Bene Gesserit", "crane", "dog", "element"};
    for (const auto& s : removals) {
        table.remove(s);
    }
    table.print();
    std::cout << std::endl << std::endl;
    return 0;
}