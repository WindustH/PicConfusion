#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;
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
  -e, --encrypt         Encrypt image
  -d, --decrypt         Decrypt image
  -t <type>, --type <type>
                        Set the encryption/decryption type
                            0: Block Confusion (MD5 Shuffle)
                            1: Pixel Confusion (MD5 Shuffle)
                            2: Row Pixel Confusion (MD5 Shuffle)
                            3: PicEncrypt Row Confusion (Logistic Map)
                            4: PicEncrypt Row+Column Confusion (Logistic Map)
                            5: Tomato Confusion (Hilbert Space-Filling Curve)
  -bx <size>, --blockX <size>
                        Set the block size in the X direction

                        Required by: 
                            0: Block Confusion (MD5 Shuffle)
  -by <size>, --blockY <size>
                        Set the block size in the Y direction

                        Required by: 
                            0: Block Confusion (MD5 Shuffle)
  -k <key>, --key <key> Set the encryption/decryption key

                        Required by: 
                            0: Block Confusion (MD5 Shuffle)
                            1: Pixel Confusion (MD5 Shuffle)
                            2: Row Pixel Confusion (MD5 Shuffle)
                            3: PicEncrypt Row Confusion (Logistic Map)
                            4: PicEncrypt Row+Column Confusion (Logistic Map)
)";

const int DEFAULT_TYPE = TYPE_HBC;
const std::string DEFAULT_OUTPUT = "output.png";
const std::string DEFAULT_KEY = "0.666";
const int DEFAULT_OPERATION = DECRYPT;
const int DEFAULT_BLOCK_SIZE_X = 50;
const int DEFAULT_BLOCK_SIZE_Y = 50;

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
    void specify(const T &data_);
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