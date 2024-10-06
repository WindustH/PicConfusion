#include "codec.hpp"
#include <algorithm>
#include <numeric>
#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/md5.h>
std::string md5(const std::string &input)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX ctx;

    // 初始化MD5上下文
    MD5_Init(&ctx);

    // 更新MD5上下文
    MD5_Update(&ctx, input.c_str(), input.size());

    // 完成MD5计算
    MD5_Final(digest, &ctx);

    // 将MD5值转换为16进制字符串
    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        hexStream << std::setw(2) << static_cast<int>(digest[i]);
    }
    return hexStream.str();
}

bool logisticSort(const std::pair<double, int> &value1, const std::pair<double, int> &value2)
{
    return value1.first < value2.first;
}

std::vector<int> shuffleArray(int arrLength, std::string seed)
{
    std::vector<int> arr(arrLength); // 创建一个大小为arrLength的vector
    std::iota(arr.begin(), arr.end(), 0); // 填充vector的元素为其索引值
    for (int i = arr.size() - 1; i > 0; i--)
    {
        int rand = std::stoi(md5(seed + std::to_string(i)).substr(0, 7), nullptr, 16) % (i + 1);
        std::swap(arr[rand],arr[i]);
    }
    return arr;
}

std::vector<std::pair<double, int>> logisticMap(double key, int size)
{
    std::vector<std::pair<double, int>> logisticArray;
    double currentValue = key;
    logisticArray.push_back({currentValue, 0});
    for (int i = 1; i < size; i++)
    {
        currentValue = 3.9999999 * currentValue * (1 - currentValue);
        logisticArray.push_back({currentValue, i});
    }
    return logisticArray;
}
// 提取索引的函数
std::vector<int> extractIndices(const std::vector<std::pair<double, int>> &pairs)
{
    std::vector<int> indices;
    for (const auto &pair : pairs)
    {
        indices.push_back(pair.second);
    }
    return indices;
}
cv::Mat encryptBC(const cv::Mat &image, const std::string key, int blockSizeX, int blockSizeY)
{
    int width = image.cols;
    int height = image.rows;
    int adjustedWidth = width;
    int adjustedHeight = height;
    cv::Mat outputImage;
    cv::Mat resizedImage;
    int x, y;
    int scaledBlockWidth, scaledBlockHeight;

    // 调整大小
    if (adjustedWidth % blockSizeX != 0)
        adjustedWidth = adjustedWidth - adjustedWidth % blockSizeX + blockSizeX;
    if (adjustedHeight % blockSizeY != 0)
        adjustedHeight = adjustedHeight - adjustedHeight % blockSizeY + blockSizeY;
    scaledBlockWidth = adjustedWidth / blockSizeX;
    scaledBlockHeight = adjustedHeight / blockSizeY;

    cv::resize(image, resizedImage, cv::Size_(adjustedWidth, adjustedHeight));
    outputImage = cv::Mat(adjustedHeight, adjustedWidth, resizedImage.type(), cv::Scalar(0, 0, 0, 0));

    std::vector<int> xMap=shuffleArray(blockSizeX, key);
    std::vector<int> yMap=shuffleArray(blockSizeY, key);

    for (int i = 0; i < adjustedWidth; i++)
    {
        for (int j = 0; j < adjustedHeight; j++)
        {
            x = i;
            y = j;
            x = (xMap[static_cast<int>(y / scaledBlockHeight)%blockSizeX] * scaledBlockWidth + x) % adjustedWidth;
            x = xMap[static_cast<int>(x / scaledBlockWidth)] * scaledBlockWidth + x % scaledBlockWidth;
            y = (yMap[static_cast<int>(x / scaledBlockWidth)%blockSizeY] * scaledBlockHeight + y) % adjustedHeight;
            y = yMap[static_cast<int>(y / scaledBlockHeight)] * scaledBlockHeight + y % scaledBlockHeight;

            outputImage.at<cv::Vec3b>(j, i) = resizedImage.at<cv::Vec3b>(y, x);
        }
    }
    return outputImage;
}
cv::Mat decryptBC(const cv::Mat &image, const std::string key, int blockSizeX, int blockSizeY)
{
    int width = image.cols;
    int height = image.rows;
    int adjustedWidth = width;
    int adjustedHeight = height;

    // 调整宽度和高度以适应块大小
    if (adjustedWidth % blockSizeX != 0)
        adjustedWidth = adjustedWidth - adjustedWidth % blockSizeX + blockSizeX;
    if (adjustedHeight % blockSizeY != 0)
        adjustedHeight = adjustedHeight - adjustedHeight % blockSizeY + blockSizeY;
    
    cv::Mat resizedImage;
    cv::Mat outputImage = cv::Mat::zeros(adjustedHeight, adjustedWidth, image.type());
    cv::resize(image, resizedImage, cv::Size_(adjustedWidth, adjustedHeight));
    // 打乱x和y映射
    std::vector<int> xMap=shuffleArray(blockSizeX, key);
    std::vector<int> yMap=shuffleArray(blockSizeY, key);
    int scaledBlockWidth = adjustedWidth / blockSizeX;
    int scaledBlockHeight = adjustedHeight / blockSizeY;
    // 解密图像
    for (int i = 0; i < adjustedWidth; i++)
    {
        for (int j = 0; j < adjustedHeight; j++)
        {
            int x = i;
            int y = j;
            x = (xMap[static_cast<int>(y / scaledBlockHeight)%blockSizeX] * scaledBlockWidth + x) % adjustedWidth;
            x = xMap[static_cast<int>(x / scaledBlockWidth)] * scaledBlockWidth + x % scaledBlockWidth;
            y = (yMap[static_cast<int>(x / scaledBlockWidth)%blockSizeY] * scaledBlockHeight + y) % adjustedHeight;
            y = yMap[static_cast<int>(y / scaledBlockHeight)] * scaledBlockHeight + y % scaledBlockHeight;
            outputImage.at<cv::Vec3b>(y, x) = resizedImage.at<cv::Vec3b>(j, i);
        }
    }

    return outputImage;
}
cv::Mat encryptPC(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage(height, width, image.type(), cv::Scalar(0, 0, 0, 0));

    std::vector<int> xMap=shuffleArray(width, key);
    std::vector<int> yMap=shuffleArray(height, key);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = i;
            int y = j;
            x = (xMap[y % width] + x) % width;
            x = xMap[x];
            y = (yMap[x % height] + y) % height;
            y = yMap[y];
            outputImage.at<cv::Vec3b>(j, i) = image.at<cv::Vec3b>(y, x);
        }
    }

    return outputImage;
}
cv::Mat decryptPC(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage = cv::Mat::zeros(height, width, image.type());

    // 打乱映射数组
    std::vector<int> xMap=shuffleArray(width, key);
    std::vector<int> yMap=shuffleArray(height, key);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = i;
            int y = j;
            x = (xMap[y % width] + x) % width;
            x = xMap[x];
            y = (yMap[x % height] + y) % height;
            y = yMap[y];

            outputImage.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(j, i);
        }
    }
    return outputImage;
}
cv::Mat encryptRPC(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(width, height, image.type(), cv::Scalar(0, 0, 0, 0));

    std::vector<int> xMap=shuffleArray(width, key);
    std::vector<int> yMap=shuffleArray(height, key);

    outputImage = cv::Mat(height, width, image.type(), cv::Scalar(0, 0, 0, 0));

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = i;
            int y = j;
            x = (xMap[y % width] + x) % width;
            x = xMap[x];
            outputImage.at<cv::Vec3b>(j, i) = image.at<cv::Vec3b>(y, x);
        }
    }

    return outputImage;
}
cv::Mat decryptRPC(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage = cv::Mat::zeros(height, width, image.type());

    std::vector<int> xMap=shuffleArray(width, key);
    std::vector<int> yMap=shuffleArray(height, key);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = i;
            int y = j;
            x = (xMap[y % width] + x) % width;
            x = xMap[x];
            outputImage.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(j, i);
        }
    }
    return outputImage;
}
cv::Mat encryptPER(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(width, height));

    cv::Mat outputImage = cv::Mat::zeros(height, width, image.type());

    std::vector<int> rowMap(width);

    // 生成 logistic 映射数组
    std::vector<std::pair<double, int>> logisticArray = logisticMap(std::stod(key), width);
    // 排序
    std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);

    // 提取索引
    rowMap = extractIndices(logisticArray);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = rowMap[i];
            int y = j;
            outputImage.at<cv::Vec3b>(j, i) = resizedImage.at<cv::Vec3b>(y, x);
        }
    }

    return outputImage;
}
cv::Mat decryptPER(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage = cv::Mat::zeros(height, width, image.type());

    std::vector<int> rowMap(width);
    // Logistic映射和打乱
    // 生成 logistic 映射数组
    std::vector<std::pair<double, int>> logisticArray = logisticMap(std::stod(key), width);
    // 排序
    std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);

    // 提取索引
    rowMap = extractIndices(logisticArray);

    // 解密图像
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = rowMap[i];
            int y = j;
            outputImage.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(j, i);
        }
    }

    return outputImage;
}
cv::Mat encryptPERC(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat tempImageData(height, width, image.type(), cv::Scalar(0, 0, 0, 0));
    cv::Mat finalImageData(height, width, image.type(), cv::Scalar(0, 0, 0, 0));
    std::vector<int> columnMap, rowMap;
    
    double currentValue = std::stod(key);

    for (int j = 0; j < height; j++)
    {

        // 生成 logistic 映射数组
        std::vector<std::pair<double, int>> logisticArray = logisticMap(currentValue, width);
        currentValue = logisticArray[width - 1].first;
        // 排序
        std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);

        // 提取索引
        std::vector<int> columnMap = extractIndices(logisticArray);
        for (int i = 0; i < width; i++)
        {
            int x = columnMap[i];
            int y = j;
            tempImageData.at<cv::Vec3b>(j, i) = image.at<cv::Vec3b>(y, x);
        }
    }
    currentValue = std::stod(key);
    for (int i = 0; i < width; i++)
    {
        // 生成 logistic 映射数组
        std::vector<std::pair<double, int>> logisticArray = logisticMap(currentValue, height);
        currentValue = logisticArray[height - 1].first;
        // 排序
        std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);

        // 提取索引
        std::vector<int> rowMap = extractIndices(logisticArray);
        for (int j = 0; j < height; j++)
        {
            int y = rowMap[j];
            int x = i;
            finalImageData.at<cv::Vec3b>(j, i) = tempImageData.at<cv::Vec3b>(y, x);
        }
    }
    return finalImageData;
}
cv::Mat decryptPERC(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat tempImageData = cv::Mat::zeros(image.size(), image.type());
    cv::Mat finalImageData = cv::Mat::zeros(image.size(), image.type());

    double currentValue = stod(key);
    std::vector<int> rowMap, columnMap;

    // 行混淆
    for (int i = 0; i < width; i++)
    {
        std::vector<std::pair<double, int>> logisticArray = logisticMap(currentValue, height);
        currentValue = logisticArray[height - 1].first;
        // 排序
        std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);

        // 提取索引
        std::vector<int> rowMap = extractIndices(logisticArray);
        for (int j = 0; j < height; j++)
        {
            int y = rowMap[j];
            int x = i;
            tempImageData.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(j, i);
        }
    }

    // 列混淆
    currentValue = stod(key);
    for (int j = 0; j < height; j++)
    {
        std::vector<std::pair<double, int>> logisticArray = logisticMap(currentValue, width);
        currentValue = logisticArray[width - 1].first;
        // 排序
        std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);

        // 提取索引
        std::vector<int> columnMap = extractIndices(logisticArray);
        for (int i = 0; i < width; i++)
        {
            int x = columnMap[i];
            int y = j;
            for (int c = 0; c < 3; c++)
                finalImageData.at<cv::Vec3b>(y, x) = tempImageData.at<cv::Vec3b>(j, i);
        }
    }

    return finalImageData;
}