#include "csvQuery.h"
#include "csvUtility.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

// Loads data from a CSV file into memory
void CSVQuery::loadFromCSV(const std::string& filename) {
    auto csvData = readFromCSV(filename);
    if (csvData.empty()) {
        throw std::runtime_error("No data found in file: " + filename);
    }
    headers = csvData[0];
    data = std::vector<std::vector<std::string>>(csvData.begin() + 1, csvData.end());
}

// Selects specified columns from the loaded data
void CSVQuery::selectColumns(const std::vector<std::string>& columns) {
    std::vector<int> columnIndexes;
    for (const auto& col : columns) {
        columnIndexes.push_back(getColumnIndex(col));
    }
    
    std::vector<std::vector<std::string>> selectedData;
    std::vector<std::string> selectedHeaders;
    
    for (auto index : columnIndexes) {
        selectedHeaders.push_back(headers[index]);
    }
    
    for (const auto& row : data) {
        std::vector<std::string> selectedRow;
        for (auto idx : columnIndexes) {
            selectedRow.push_back(row[idx]);
        }
        selectedData.push_back(selectedRow);
    }
    
    headers = selectedHeaders;
    data = selectedData;
}

// Limits the number of rows in the data
void CSVQuery::takeRows(int num_rows) {
    if (num_rows > static_cast<int>(data.size())) {
        throw std::runtime_error("Requested number of rows exceeds available rows.");
    }
    data.resize(num_rows);
}

// Sorts data by the specified column in decreasing order
void CSVQuery::orderByColumn(const std::string& column) {
    int index = getColumnIndex(column);
    sortByColumn(data, index, false); // Sorting in decreasing order
}

// Joins current data with data from another CSV file based on a common column
void CSVQuery::joinWithCSV(const std::string& filename, const std::string& column) {
    std::vector<std::vector<std::string>> joinData = readFromCSV(filename);
    if (joinData.empty()) {
        throw std::runtime_error("No data found in file: " + filename);
    }
    
    std::vector<std::string> joinHeaders = joinData[0];
    joinData.erase(joinData.begin()); // Remove header from data
    
    int currentDataColumn = getColumnIndex(column);
    int joinDataColumn = getColumnIndex(column, joinHeaders);
    
    sortByColumn(data, currentDataColumn, true);
    sortByColumn(joinData, joinDataColumn, true);
    
    std::vector<std::vector<std::string>> combinedData;
    
    for (const auto& row : data) {
        int joinIndex = 0;
        while (joinIndex < static_cast<int>(joinData.size()) && row[currentDataColumn] != joinData[joinIndex][joinDataColumn]) {
            joinIndex++;
        }
        
        int tempIndex = joinIndex;
        while (tempIndex < static_cast<int>(joinData.size()) && row[currentDataColumn] == joinData[tempIndex][joinDataColumn]) {
            std::vector<std::string> combinedRow = row;
            for (int i = 0; i < static_cast<int>(joinData[tempIndex].size()); i++) {
                if (i != joinDataColumn) {
                    combinedRow.push_back(joinData[tempIndex][i]);
                }
            }
            combinedData.push_back(combinedRow);
            tempIndex++;
        }
    }
    
    data = combinedData;
    
    for (const auto& header : joinHeaders) {
        if (header != column) {
            headers.push_back(header);
        }
    }
    
    sortByColumn(data, 0, true); // Sort by the first column after join
}

// Performs counting based on the unique values in a column
void CSVQuery::countByColumn(const std::string& column) {
    int index = getColumnIndex(column);
    std::unordered_map<std::string, int> countMap;
    
    for (auto& row : data) {
        countMap[row[index]]++;
    }
    
    headers = {column, "count"};
    data.clear();
    
    for (const auto& pairCount : countMap) {
        data.push_back({pairCount.first, std::to_string(pairCount.second)});
    }
}

// Helper function to get the index of a column by name
int CSVQuery::getColumnIndex(const std::string& column, const std::vector<std::string>& checkHeaders) const {
    const auto& headersToCheck = checkHeaders.empty() ? headers : checkHeaders;
    auto it = std::find(headersToCheck.begin(), headersToCheck.end(), column);
    if (it == headersToCheck.end()) {
        throw std::runtime_error("Column not found:- " + column);
    }
    return std::distance(headersToCheck.begin(), it);
}

// Helper function to sort data by a specific column index
void CSVQuery::sortByColumn(std::vector<std::vector<std::string>>& data, const int& columnIndex, bool ascending) {
    if (!data.empty() && std::all_of(data[0][columnIndex].begin(), data[0][columnIndex].end(), ::isdigit)) {
        std::sort(data.begin(), data.end(), [columnIndex, ascending](const std::vector<std::string>& a, const std::vector<std::string>& b) {
            if (ascending) {
                return std::stoi(a[columnIndex]) < std::stoi(b[columnIndex]);
            } else {
                return std::stoi(a[columnIndex]) > std::stoi(b[columnIndex]);
            }
        });
    } else {
        std::sort(data.begin(), data.end(), [columnIndex, ascending](const std::vector<std::string>& a, const std::vector<std::string>& b) {
            if (ascending) {
                return a[columnIndex] < b[columnIndex];
            } else {
                return a[columnIndex] > b[columnIndex];
            }
        });
    }
}


// Accessor methods to retrieve data and headers
std::vector<std::vector<std::string>> CSVQuery::getData() const {
    return data;
}

std::vector<std::string> CSVQuery::getHeaders() const {
    return headers;
}
