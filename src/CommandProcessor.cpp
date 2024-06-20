#include "CommandProcessor.h"
#include <stdexcept>
#include <iostream>

CommandProcessor::CommandProcessor(CSVQuery& engine) : engine(engine) {}

void CommandProcessor::processCommands(const std::vector<std::string>& commands)
{
    try {
        // Iterate over each command in the commands vector
        for (size_t i = 0; i < commands.size(); ++i)
        {
            // Handle the FROM command
            if (commands[i] == "FROM") 
            {
                if (i + 1 < commands.size()) {
                    // Check if the next argument exists and process the filename
                    engine.loadFromCSV(commands[++i]);
                } else {
                    throw std::invalid_argument("Missing filename after FROM command.");
                }
            }
            // Handle the SELECT command
            else if (commands[i] == "SELECT")
            {
                if (i + 1 < commands.size()) {
                    // Check if the next argument exists and process the column names
                    engine.selectColumns(splitData(commands[++i],','));
                } else {
                    throw std::invalid_argument("Missing column names after SELECT command.");
                }
            }
            // Handle the TAKE command
            else if (commands[i] == "TAKE")
            {
                if (i + 1 < commands.size()) {
                    // Check if the next argument exists and convert to integer
                    engine.takeRows(std::stoi(commands[++i]));
                } else {
                    throw std::invalid_argument("Missing number of rows after TAKE command.");
                }
            }
            // Handle the ORDERBY command
            else if (commands[i] == "ORDERBY")
            {
                if (i + 1 < commands.size()) {
                    // Check if the next argument exists and process the column name
                    engine.orderByColumn(commands[++i]);
                } else {
                    throw std::invalid_argument("Missing column name after ORDERBY command.");
                }
            }
            // Handle the JOIN command
            else if (commands[i] == "JOIN")
            {
                if (i + 2 < commands.size()) {
                    // Check if the next two arguments exist: filename and column name
                    std::string File = commands[++i];
                    std::string Column = commands[++i];
                    engine.joinWithCSV(File, Column);
                } else {
                    throw std::invalid_argument("Insufficient arguments after JOIN command (expected filename and column name).");
                }
            }
            // Handle the COUNTBY command
            else if (commands[i] == "COUNTBY")
            {
                if (i + 1 < commands.size()) {
                    // Check if the next argument exists and process the column name
                    engine.countByColumn(commands[++i]);
                } else {
                    throw std::invalid_argument("Missing column name after COUNTBY command.");
                }
            }
            // Handle unknown commands
            else {
                throw std::invalid_argument("Unknown command or missing arguments.");
            }
        }

        // Output headers
        const auto& headers = engine.getHeaders();
        for (const auto& header : headers)
        {
            std::cout << header << (header == headers.back() ? "\n" : ",");
        }

        // Output data
        const auto& data = engine.getData();
        for (const auto& row : data)
        {
            for (const auto& field : row)
            {
                std::cout << field << (field == row.back() ? "\n" : ",");
            }
        }
    } 
    // Catch invalid argument errors
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument error: " << e.what() << std::endl;
    }
    // Catch all other standard exceptions
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    // Catch any other exceptions
    catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
    }
}
