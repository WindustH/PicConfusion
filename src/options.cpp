#include "options.hpp"
#include <iostream>

template <typename T>
Option<T>::Option(const std::vector<int> &requiredBy_) : requiredBy(requiredBy_)
{
}

template <typename T>
Option<T>::Option(const std::vector<int> &requiredBy_, const T &data_) : requiredBy(requiredBy_), data(data_)
{
}

template <typename T>
void Option<T>::specify(const T &data_)
{
    data=data_;
    specified=true;
}

Options::Options(int argc, char *argv[]) : type(Option<int>(TYPE_ALL, DEFAULT_TYPE)),
                                           input(Option<std::string>(TYPE_ALL)),
                                           output(Option<std::string>(TYPE_ALL, DEFAULT_OUTPUT)),
                                           key(Option<std::string>({TYPE_MD5_B, TYPE_MD5_P, TYPE_MD5_CP, TYPE_LG_R, TYPE_LG_RC}, DEFAULT_KEY)),
                                           operation(Option<int>(TYPE_ALL, DEFAULT_OPERATION)),
                                           blockSizeX(Option<int>({TYPE_MD5_B}, DEFAULT_BLOCK_SIZE_X)),
                                           blockSizeY(Option<int>({TYPE_MD5_B}, DEFAULT_BLOCK_SIZE_Y)),
                                           help(Option<bool>({}, false))
{
    parse(argc, argv);
    check();
}

void Options::parse(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        std::string option(argv[i]);
        if (option == "-h" || option == "--help")
            help.specify(true);
        else if (option == "-o" || option == "--output")
        {
            i++;
            output.specify(std::string(argv[i]));
        }
        else if (option == "-i" || option == "--input")
        {
            i++;
            input.specify(std::string(argv[i]));
        }
        else if (option == "-d" || option == "--decrypt")
            operation.specify(DECRYPT);
        else if (option == "-e" || option == "--encrypt")
            operation.specify(ENCRYPT);
        else if (option == "-t" || option == "--type")
        {
            i++;
            type.specify(std::stoi(argv[i]));
        }
        else if (option == "-bx" || option == "--blockSizeX")
        {
            i++;
            blockSizeX.specify(std::stoi(argv[i]));
        }
        else if (option == "-by" || option == "--blockSizeY")
        {
            i++;
            blockSizeY.specify(std::stoi(argv[i]));
        }
        else if (option == "-k" || option == "--key")
        {
            i++;
            key.specify(std::string(argv[i]));
        }
    }
}

void Options::check()
{
    fs::path input_path = input.data;
    if (help.data)
        return;
    if (!input.specified)
        throw std::invalid_argument("Input path wasn't specified!\n");
    if (!fs::exists(input_path))
        throw std::invalid_argument("Input path doesn't exist!\n");
    auto type_it = std::find(TYPE_ALL.begin(), TYPE_ALL.end(), type.data);
    if (type_it == TYPE_ALL.end())
        throw std::invalid_argument("Codec type doesn't exist!\n");

    if (blockSizeX.data <= 0 || blockSizeY.data <= 0)
        throw std::invalid_argument("Blocksize should be greater than 0!\n");

    if (!type.specified)
        std::cout << "Type wasn't specified. Using the default type " << type.data << ".\n";

    type_it = std::find(key.requiredBy.begin(), key.requiredBy.end(), type.data);
    if (!key.specified && type_it != key.requiredBy.end())
        std::cout << "Key wasn't specified. Using the default key " << key.data << ".\n";

    type_it = std::find(output.requiredBy.begin(), output.requiredBy.end(), type.data);
    if (!output.specified && type_it != output.requiredBy.end())
        std::cout << "Output path wasn't specified. Using the default output path " << output.data << ".\n";

    type_it = std::find(operation.requiredBy.begin(), operation.requiredBy.end(), type.data);
    if (!operation.specified && type_it != operation.requiredBy.end())
        std::cout << "Operation wasn't specified. Using the default operation " << operation.data << ".\n";

    type_it = std::find(blockSizeX.requiredBy.begin(), blockSizeX.requiredBy.end(), type.data);
    if (!blockSizeX.specified && type_it != blockSizeX.requiredBy.end())
        std::cout << "BlockSizeX wasn't specified. Using the default blockSizeX " << blockSizeX.data << ".\n";

    type_it = std::find(blockSizeY.requiredBy.begin(), blockSizeY.requiredBy.end(), type.data);
    if (!blockSizeY.specified && type_it != blockSizeY.requiredBy.end())
        std::cout << "BlockSizeY wasn't specified. Using the default blockSizeY " << blockSizeY.data << ".\n";
}