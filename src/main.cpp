#include <codec.hpp>
#include <iostream>
#include <stdexcept>

const int OPERATE_ENCRYPT = 0;
const int OPERATE_DECRYPT = 1;

const int TYPE_BC = 0;   // 方块混淆
const int TYPE_PC = 1;   // 像素混淆
const int TYPE_RPC = 2;  // 行像素混淆
const int TYPE_PER = 3;  // PicEncrypt 行混淆
const int TYPE_PERC = 4; // PicEncrypt 行+列混淆
const std::string HELP_INFO=
R"(
PicConfusion Codec by Windust
Usage: main [options]...

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
                        3: PicEncrypt Row Confusion
                        4: PicEncrypt Row+Column Confusion
  -bx <size>, --blockX <size>
                        Set the block size in the X direction
  -by <size>, --blockY <size>
                        Set the block size in the Y direction
  -k <key>, --key <key>  Set the encryption/decryption key

Examples:
  Encrypt image:
    picencrypt -i input.png -o output.png -e -t 0 -bx 50 -by 50 -k 0.666

  Decrypt image:
    picencrypt -i input.png -o output.png -d -t 0 -bx 50 -by 50 -k 0.666

Note:
1. For Block Confusion type (TYPE_BC), the block size (-bx, -by) must be greater than 0.
2. For other types, the block size arguments will be ignored.
3. The key (-k) is a string that may require a specific format depending on the encryption/decryption type.
4. The input image (-i) must be a valid image file path.
5. The output image (-o) can be any path where you want to save the encrypted/decrypted result.
)";

cv::Mat run(const int &operation, const int &type, const cv::Mat &image,const std::string &key, int blockSizeX, int blockSizeY)
{
    if (operation == OPERATE_ENCRYPT)
    {
        switch (type)
        {
        case TYPE_BC:
            if (blockSizeX <= 0 || blockSizeY <= 0)
                throw(std::invalid_argument("Blocksize should be greater than 0 !\n"));
            return encryptBC(image, key, blockSizeX, blockSizeY);
        case TYPE_PC:
            return encryptPC(image, key);
        case TYPE_RPC:
            return encryptPC(image, key);
        case TYPE_PER:
            return encryptPER(image, key);
        case TYPE_PERC:
            return encryptPERC(image, key);
        default:
            throw(std::invalid_argument("Encrypt type doesn't exist !\n"));
        }
    }
    if (operation == OPERATE_DECRYPT)
    {
        switch (type)
        {
        case TYPE_BC:
            if (blockSizeX <= 0 || blockSizeY <= 0)
                throw(std::invalid_argument("Blocksize should be greater than 0 !\n"));
            return decryptBC(image, key, blockSizeX, blockSizeY);
        case TYPE_PC:
            return decryptPC(image, key);
        case TYPE_RPC:
            return decryptPC(image, key);
        case TYPE_PER:
            return decryptPER(image, key);
        case TYPE_PERC:
            return decryptPERC(image, key);
        default:
            throw(std::invalid_argument("Decrypt type doesn't exist !\n"));
        }
    }
    throw(std::invalid_argument("Operation doesn't exist !\n"));
}

int main(int argc, char *argv[])
{
    std::string input_path ;
    std::string output_path ;
    int operation=0;
    int type=0;
    std::string key="0.666";
    int blockSizeX=50;
    int blockSizeY=50;
    if (argc<=1)
    {
        std::cout<<HELP_INFO;
        return 0;
    }
    for(int i=1;i<argc;i++)
    {
        std::string option(argv[i]);
        if(option=="-h" || option=="--help")
        {
            std::cout<<HELP_INFO;
            return 0;
        }
        if(option=="-o" || option=="--output")
        {
            i++;
            output_path=std::string(argv[i]);
        }
        if(option=="-i" || option=="--input")
        {
            i++;
            input_path=std::string(argv[i]);
        }
        if(option=="-d" || option=="--decrypt") operation=OPERATE_DECRYPT; 
        if(option=="-e" || option=="--encrypt") operation=OPERATE_ENCRYPT;
        if(option=="-t" || option=="--type")
        {
            i++;
            type=std::stoi(argv[i]);
        }
        if(option=="-bx" || option=="--blockSizeX")
        {
            i++;
            blockSizeX=std::stoi(argv[i]);
        }
        if(option=="-by" || option=="--blockSizeY")
        {
            i++;
            blockSizeY=std::stoi(argv[i]);
        }
        if(option=="-k" || option=="--key")
        {
            i++;
            key=std::string(argv[i]);
        }
    }
    cv::Mat input_image=cv::imread(input_path, cv::IMREAD_COLOR);
    cv::Mat output_image=run(operation,type,input_image,key,blockSizeX,blockSizeY);
    int compression_level = 9;  // 设置压缩级别
    bool result = cv::imwrite(output_path, output_image, std::vector<int>{cv::IMWRITE_PNG_COMPRESSION, compression_level});
    return 0;
}