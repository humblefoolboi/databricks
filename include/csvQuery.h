#pragma once

#include <vector>
#include <string>

class CSVQuery {
public:
    // Methods for performing SQL-like operations
    void loadFromCSV(const std::string& filename);
    void selectColumns(const std::vector<std::string>& columns);
    void takeRows(int num_rows);
    void orderByColumn(const std::string& column);
    void joinWithCSV(const std::string& filename, const std::string& column);
    void countByColumn(const std::string& column);

    // Accessors
    std::vector<std::vector<std::string>> getData() const;
    std::vector<std::string> getHeaders() const;

private:
    std::vector<std::vector<std::string>> data;     // Holds table data
    std::vector<std::string> headers;               // Holds column headers

    // Helper methods
    int getColumnIndex(const std::string& column, const std::vector<std::string>& checkHeaders = {}) const;
    void sortByColumn(std::vector<std::vector<std::string>>& data, const int& columnIndex, bool ascending = true);
};
