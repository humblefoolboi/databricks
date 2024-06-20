#pragma once 

#include <vector>
#include <string>

std::vector<std::string> splitData(const std::string& str, char delimiter);
std::vector<std::vector<std::string>> readFromCSV(const std::string& filename);
