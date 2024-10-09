#include "options.hpp"
#include <iostream>

const int ENCRYPT = 0;
const int DECRYPT = 1;
const int RESTORE = -1;

const int TYPE_MD5_B = 0;  // 方块混淆
const int TYPE_MD5_P = 1;  // 像素混淆
const int TYPE_MD5_CP = 2; // 行像素混淆
const int TYPE_LG_R = 3;   // PicEncrypt 行混淆
const int TYPE_LG_RC = 4;  // PicEncrypt 行+列混淆
const int TYPE_HBC = 5;    // 番茄混淆

const std::vector<int> TYPE_ALL = {
    TYPE_MD5_B,
    TYPE_MD5_P,
    TYPE_MD5_CP,
    TYPE_LG_R,
    TYPE_LG_RC,
    TYPE_HBC};

const std::string HELP_INFO =
    R"(
PicConfusion Codec by Windust

Usage: PicConfusion [options]...

Options:
  -h, --help            Display help information and exit
  -i <path>, --input <path>
                        Set the input image path
  -o <path>, --output <path>
                        Set the output image path
  -e, --encrypt          Encrypt operation
  -d, --decrypt          Decrypt operation
  -t <type>, --type <type>
                        Set the encryption/decryption type
                        0: Block Confusion
                        1: Pixel Confusion
                        2: Row Pixel Confusion
                        3: PicEncrypt Row Confusion ()
                        4: PicEncrypt Row+Column Confusion
                        5: Tomato Confusion (Hilbert Curve)
  -bx <size>, --blockX <size>
                        Set the block size in the X direction
  -by <size>, --blockY <size>
                        Set the block size in the Y direction
  -k <key>, --key <key>  Set the encryption/decryption key
)";

const int DEFAULT_TYPE = TYPE_HBC;
const std::string DEFAULT_OUTPUT = "output.png";
const std::string DEFAULT_KEY = "0.666";
const int DEFAULT_OPERATION = DECRYPT;
const int DEFAULT_BLOCK_SIZE_X = 50;
const int DEFAULT_BLOCK_SIZE_Y = 50;

template <typename T>
Option<T>::Option(const std::vector<int> &requiredBy_) : requiredBy(requiredBy_)
{
}

template <typename T>
Option<T>::Option(const std::vector<int> &requiredBy_, const T &data_) : requiredBy(requiredBy_), data(data_), specified(true)
{
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
            help.data = true;
        else if (option == "-o" || option == "--output")
        {
            i++;
            output.data = std::string(argv[i]);
        }
        else if (option == "-i" || option == "--input")
        {
            i++;
            input.data = std::string(argv[i]);
        }
        else if (option == "-d" || option == "--decrypt")
            operation.data = DECRYPT;
        else if (option == "-e" || option == "--encrypt")
            operation.data = ENCRYPT;
        else if (option == "-t" || option == "--type")
        {
            i++;
            type.data = std::stoi(argv[i]);
        }
        else if (option == "-bx" || option == "--blockSizeX")
        {
            i++;
            blockSizeX.data = std::stoi(argv[i]);
        }
        else if (option == "-by" || option == "--blockSizeY")
        {
            i++;
            blockSizeY.data = std::stoi(argv[i]);
        }
        else if (option == "-k" || option == "--key")
        {
            i++;
            key.data = std::string(argv[i]);
        }
    }
}

void Options::check()
{
    fs::path input_path = input.data;
    if (help.data)
        return;
    if (!input.specified || !fs::exists(input_path))
        throw std::invalid_argument("Input path doesn't exist or wasn't specified!\n");
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
    if (!output.specified && type_it != key.requiredBy.end())
        std::cout << "Output path wasn't specified. Using the default output path " << output.data << ".\n";

    type_it = std::find(operation.requiredBy.begin(), operation.requiredBy.end(), type.data);
    if (!operation.specified && type_it != key.requiredBy.end())
        std::cout << "Operation wasn't specified. Using the default operation " << operation.data << ".\n";

    type_it = std::find(blockSizeX.requiredBy.begin(), blockSizeX.requiredBy.end(), type.data);
    if (!blockSizeX.specified && type_it != key.requiredBy.end())
        std::cout << "BlockSizeX wasn't specified. Using the default blockSizeX " << blockSizeX.data << ".\n";

    type_it = std::find(blockSizeY.requiredBy.begin(), blockSizeY.requiredBy.end(), type.data);
    if (!blockSizeY.specified && type_it != key.requiredBy.end())
        std::cout << "BlockSizeY wasn't specified. Using the default blockSizeY " << blockSizeY.data << ".\n";
}