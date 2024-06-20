#include "csvUtility.h"
#include <fstream>
#include <sstream>

// Splits a string into tokens based on delimiter
std::vector<std::string> splitData(const std::string& str, char delimiter) {
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;
    
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// Reads CSV data from a file and returns as a vector of vectors of strings
std::vector<std::vector<std::string>> readFromCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream csvFile(filename);
    
    if (!csvFile) {
        throw std::runtime_error("Error: Unable to open file " + filename);
    }
    
    std::string csvLine;
    while (std::getline(csvFile, csvLine)) {
        data.push_back(splitData(csvLine, ','));
    }
    
    csvFile.close();
    return data;
}
