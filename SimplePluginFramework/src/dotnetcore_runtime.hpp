
#include "exec.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <locale>

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
 
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
 
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

std::vector<std::string> splitter(std::string in_pattern, std::string& content)
{
    std::vector<std::string> split_content;

    std::regex pattern(in_pattern);
    std::copy(std::sregex_token_iterator(content.begin(), content.end(), pattern, -1),
    std::sregex_token_iterator(),back_inserter(split_content));
    return split_content;
}

std::string GetDotNetCoreRuntimePath()
{
    std::string runtimes = exec("dotnet --list-runtimes");

    //get last line
    std::vector<std::string> values = splitter("\n", runtimes);
    std::string runtime = values[values.size() - 1];

    values = splitter(" ", runtime);
    std::string directory = values[2];
    directory = trim(directory, "[]") + "/" + values[1];
    return directory;
}