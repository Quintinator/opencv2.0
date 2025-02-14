#include <opencv2/opencv.hpp>
#include <iostream>
#include "detector.hpp"
#include "batchParser.hpp"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        BatchParser batchParser;
        std::string batchFile = argv[1];
        batchParser.processBatchFile("batch.txt");
    }
    else
    {
        Detector detector;
        detector.InteractiveMode();
    }
    return 0;
}
