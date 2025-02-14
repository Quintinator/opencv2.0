#ifndef BATCHPARSER_H
#define BATCHPARSER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <string>
#include <chrono>
#include <iterator>
#include <numeric>

#include <opencv2/opencv.hpp>
#include "detector.hpp"

/**
 * @class BatchParser
 * @brief Processes batch files containing shape detection commands.
 *
 * The BatchParser class is designed to read a file containing multiple shape
 * detection commands, process each command, and invoke the shape detection
 * functionality accordingly. Each line in the file represents a separate command
 * specifying a shape and its color to be detected.
 */
class BatchParser
{
public:
    BatchParser();
    virtual ~BatchParser();

    /**
     * @brief Processes a batch file containing shape detection commands.
     *
     * This method reads a specified file line by line, parsing each line to extract
     * shape detection commands. Each command specifies a shape and a color. The method
     * handles comments and invalid lines gracefully, ignoring them. For each valid command,
     * it invokes the shape detection functionality of the Detector class.
     *
     * Lines can contain comments (starting with '#'), and inline comments are supported.
     * The shape and color are case-insensitively processed to ensure flexibility in command formatting.
     *
     * @param filePath The path to the batch file containing shape detection commands.
     */
    void processBatchFile(const std::string filePath);
};

#endif