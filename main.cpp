#include <iostream>
#include <vector>
#include <string>
#include "csvQuery.h"
#include "CommandProcessor.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> ..." << std::endl;
        return 1;
    }

    // Convert command line arguments into a vector of strings
    std::vector<std::string> commands(argv + 1, argv + argc);
    CSVQuery engine;
    CommandProcessor processor(engine);

    // Process commands using the CommandProcessor and handle exceptions
    processor.processCommands(commands);

    return 0;
}
