#include "parser.h"
#include <sstream>

std::vector<std::string> splitCommand(const std::string& input)
{
    std::stringstream ss(input);
    std::string word;
    std::vector<std::string> tokens;

    while(ss >> word)
        tokens.push_back(word);

    return tokens;
}