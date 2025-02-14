#include "batchParser.hpp"

BatchParser::BatchParser()
{
}

BatchParser::~BatchParser()
{
}

void BatchParser::processBatchFile(const std::string filePath)
{
    Detector detector;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
        {
            line = line.substr(0, commentPos);
        }

        std::istringstream iss(line);
        std::vector<std::string> words(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

        if (words.size() < 2)
            continue;

        std::string color = words.back();
        words.pop_back();
        std::string shape = std::accumulate(std::next(words.begin()), words.end(), words[0],
                                            [](std::string a, std::string b)
                                            { return std::move(a) + ' ' + std::move(b); });

        std::transform(shape.begin(), shape.end(), shape.begin(), ::tolower);
        std::transform(color.begin(), color.end(), color.begin(), ::tolower);
        detector.BatchMode(shape, color);

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}
