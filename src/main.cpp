#include "codec.hpp"
#include "options.hpp"
#include <iomanip>
#include <fstream>

bool fixPNG(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    // Move to the end of the file
    std::streampos fileSize;
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(-2, std::ios::end);

    // Check if the file has at least 2 bytes
    if (fileSize < 2)
    {
        std::cerr << "File is too small to have 2 bytes at the end." << std::endl;
        file.close();
        return false;
    }

    // Read the last two bytes
    unsigned char lastTwoBytes[2] = {0, 0};
    file.read(reinterpret_cast<char *>(lastTwoBytes), 2);
    file.close();

    // Output the last two bytes in hexadecimal format
    std::cout << "Last two bytes in hexadecimal: 0x"
              << std::hex << std::uppercase << (int)lastTwoBytes[0]
              << " 0x" << std::hex << std::uppercase << (int)lastTwoBytes[1]
              << std::endl;

    // Check if they are AE 42
    if (lastTwoBytes[0] == 0xAE && lastTwoBytes[1] == 0x42)
    {
        std::ofstream outFile(filename, std::ios::binary | std::ios::app);
        if (!outFile.is_open())
        {
            std::cerr << "Unable to open file to append: " << filename << std::endl;
            return false;
        }

        // Append 60 82
        outFile.write("\x60\x82", 2);
        outFile.close();
        return true;
    }

    return false;
}

bool run(Options opts)
{
    if (opts.help.data) {
        std::cout<<HELP_INFO;
        return true;
    }
    if (fixPNG(opts.input.data))
    {
        std::cout << "File has been updated." << std::endl;
    }
    else
    {
        std::cout << "File does not end with AE 42, no update performed." << std::endl;
    }
    cv::Mat input_image = cv::imread(opts.input.data, cv::IMREAD_COLOR);
    cv::Mat output_image;

    if (opts.operation.data == ENCRYPT)
    {
        switch (opts.type.data)
        {
        case TYPE_MD5_B:
            output_image = MD5_B_enc(input_image, opts.key.data, opts.blockSizeX.data, opts.blockSizeY.data);break;
        case TYPE_MD5_P:
            output_image = MD5_P_enc(input_image, opts.key.data);break;
        case TYPE_MD5_CP:
            output_image = MD5_RP_enc(input_image, opts.key.data);break;
        case TYPE_LG_R:
            output_image = LG_R_enc(input_image, opts.key.data);break;
        case TYPE_LG_RC:
            output_image = LG_RC_enc(input_image, opts.key.data);break;
        case TYPE_HBC:
            output_image = HBC_enc(input_image);break;
        }
    }
    else if (opts.operation.data == DECRYPT)
    {
        switch (opts.type.data)
        {
        case TYPE_MD5_B:
            output_image = MD5_B_dec(input_image, opts.key.data, opts.blockSizeX.data, opts.blockSizeY.data);break;
        case TYPE_MD5_P:
            output_image = MD5_P_dec(input_image, opts.key.data);break;
        case TYPE_MD5_CP:
            output_image = MD5_RP_dec(input_image, opts.key.data);break;
        case TYPE_LG_R:
            output_image = LG_R_dec(input_image, opts.key.data);break;
        case TYPE_LG_RC:
            output_image = LG_RC_dec(input_image, opts.key.data);break;
        case TYPE_HBC:
            output_image = HBC_dec(input_image);break;
        }
    }

    int compression_level = 9;
    bool result = cv::imwrite(opts.output.data, output_image, std::vector<int>{cv::IMWRITE_PNG_COMPRESSION, compression_level});
    return result;
}

int main(int argc, char *argv[])
{
    Options opts(argc, argv);
    run(opts);
    return 0;
}