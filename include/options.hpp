#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

template <typename T>
class Option
{
private:
public:
    std::vector<int> requiredBy;
    bool specified = false;
    T data;
    Option(const std::vector<int> &requiredBy_);
    Option(const std::vector<int> &requiredBy_, const T &data_);
};

class Options
{
public:
    Option<int> type;
    Option<std::string> input;
    Option<std::string> output;
    Option<std::string> key;
    Option<int> operation;
    Option<int> blockSizeX;
    Option<int> blockSizeY;
    Option<bool> help;

    Options(int argc, char *argv[]);
    void parse(int argc, char *argv[]);
    void check();
};



#endif // OPTIONS_HPP