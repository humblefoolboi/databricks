#pragma once

#include <string>
#include <vector>
#include "csvQuery.h"
#include "csvUtility.h"

class CommandProcessor {
public:
    // Constructor accepting a reference to CSVQuery engine
    CommandProcessor(CSVQuery& engine);

    // Process a vector of commands
    void processCommands(const std::vector<std::string>& commands);

private:
    CSVQuery& engine; // Reference to CSVQuery engine for processing
};
