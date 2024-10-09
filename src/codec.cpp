#include "codec.hpp"

// 番茄混淆编码
cv::Mat HBC_enc(const cv::Mat &image)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(image.size(), image.type());

    std::vector<std::pair<int, int>> hilbertArray;
    hilbertMap(hilbertArray, width, height);

    int offset = std::round((std::sqrt(5) - 1) / 2 * width * height);

    for (int k = 0; k < width * height; k++)
    {
        int x = hilbertArray[k].first;
        int y = hilbertArray[k].second;
        int i = hilbertArray[(k + offset) % (width * height)].first;
        int j = hilbertArray[(k + offset) % (width * height)].second;
        outputImage.at<cv::Vec3b>(j, i) = image.at<cv::Vec3b>(y, x);
    }
    return outputImage;
}
// 番茄混淆解码
cv::Mat HBC_dec(const cv::Mat &image)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage(image.size(), image.type());

    std::vector<std::pair<int, int>> hilbertArray;
    hilbertMap(hilbertArray, width, height);

    int offset = std::round((std::sqrt(5) - 1) / 2 * width * height);

    for (int k = 0; k < width * height; k++)
    {
        int x = hilbertArray[k].first;
        int y = hilbertArray[k].second;
        int i = hilbertArray[(k + offset) % (width * height)].first;
        int j = hilbertArray[(k + offset) % (width * height)].second;
        outputImage.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(j, i);
    }
    return outputImage;
}
// 方块混淆编码
cv::Mat MD5_B_enc(const cv::Mat &image, const std::string &key, const int &blockSizeX, const int &blockSizeY)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(height, width, image.type());

    if (width % blockSizeX != 0)
        width = width - width % blockSizeX + blockSizeX;
    if (height % blockSizeY != 0)
        height = height - height % blockSizeY + blockSizeY;

    int blockWidth = width / blockSizeX;
    int blockHeight = height / blockSizeY;

    std::vector<int> xMap = md5Shuffle(blockSizeX, key);
    std::vector<int> yMap = md5Shuffle(blockSizeY, key);

    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(width, height));

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = i;
            int y = j;
            x = (xMap[static_cast<int>(y / blockHeight) % blockSizeX] * blockWidth + x) % width;
            x = xMap[static_cast<int>(x / blockWidth)] * blockWidth + x % blockWidth;
            y = (yMap[static_cast<int>(x / blockWidth) % blockSizeY] * blockHeight + y) % height;
            y = yMap[static_cast<int>(y / blockHeight)] * blockHeight + y % blockHeight;

            outputImage.at<cv::Vec3b>(j, i) = resizedImage.at<cv::Vec3b>(y, x);
        }
    }
    return outputImage;
}
// 方块混淆解码
cv::Mat MD5_B_dec(const cv::Mat &image, const std::string &key, const int &blockSizeX, const int &blockSizeY)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(height, width, image.type());

    if (width % blockSizeX != 0)
        width = width - width % blockSizeX + blockSizeX;
    if (height % blockSizeY != 0)
        height = height - height % blockSizeY + blockSizeY;

    int blockWidth = width / blockSizeX;
    int blockHeight = height / blockSizeY;

    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(width, height));

    std::vector<int> xMap = md5Shuffle(blockSizeX, key);
    std::vector<int> yMap = md5Shuffle(blockSizeY, key);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = i;
            int y = j;
            x = (xMap[static_cast<int>(y / blockHeight) % blockSizeX] * blockWidth + x) % width;
            x = xMap[static_cast<int>(x / blockWidth)] * blockWidth + x % blockWidth;
            y = (yMap[static_cast<int>(x / blockWidth) % blockSizeY] * blockHeight + y) % height;
            y = yMap[static_cast<int>(y / blockHeight)] * blockHeight + y % blockHeight;
            outputImage.at<cv::Vec3b>(y, x) = resizedImage.at<cv::Vec3b>(j, i);
        }
    }

    return outputImage;
}
// 像素混淆编码
cv::Mat MD5_P_enc(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage(image.size(), image.type());

    std::vector<int> xMap = md5Shuffle(width, key);
    std::vector<int> yMap = md5Shuffle(height, key);

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
// 像素混淆解码
cv::Mat MD5_P_dec(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage(image.size(), image.type());

    // 打乱映射数组
    std::vector<int> xMap = md5Shuffle(width, key);
    std::vector<int> yMap = md5Shuffle(height, key);

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
// 行像素混淆编码
cv::Mat MD5_RP_enc(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(image.size(), image.type());

    std::vector<int> xMap = md5Shuffle(width, key);
    std::vector<int> yMap = md5Shuffle(height, key);

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
// 行像素混淆解码
cv::Mat MD5_RP_dec(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(image.size(), image.type());

    std::vector<int> xMap = md5Shuffle(width, key);
    std::vector<int> yMap = md5Shuffle(height, key);

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
// PicEncrypt 行混淆编码
cv::Mat LG_R_enc(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage(image.size(), image.type());

    std::vector<int> rowMap(width);
    std::vector<std::pair<double, int>> logisticArray = logisticMap(std::stod(key), width);
    std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);
    rowMap = extractIndices(logisticArray);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int x = rowMap[i];
            int y = j;
            outputImage.at<cv::Vec3b>(j, i) = image.at<cv::Vec3b>(y, x);
        }
    }

    return outputImage;
}
// PicEncrypt 行混淆解码
cv::Mat LG_R_dec(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(image.size(), image.type());

    std::vector<int> rowMap(width);
    std::vector<std::pair<double, int>> logisticArray = logisticMap(std::stod(key), width);
    std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);
    rowMap = extractIndices(logisticArray);

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
// PicEncrypt 行+列混淆编码
cv::Mat LG_RC_enc(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat tempImage(image.size(), image.type());
    cv::Mat outputImage(image.size(), image.type());

    double currentValue = std::stod(key);

    for (int j = 0; j < height; j++)
    {

        std::vector<std::pair<double, int>> logisticArray = logisticMap(currentValue, width);
        currentValue = logisticArray[width - 1].first;
        std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);
        std::vector<int> columnMap = extractIndices(logisticArray);

        for (int i = 0; i < width; i++)
        {
            int x = columnMap[i];
            int y = j;
            tempImage.at<cv::Vec3b>(j, i) = image.at<cv::Vec3b>(y, x);
        }
    }
    currentValue = std::stod(key);
    for (int i = 0; i < width; i++)
    {
        std::vector<std::pair<double, int>> logisticArray = logisticMap(currentValue, height);
        currentValue = logisticArray[height - 1].first;
        std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);
        std::vector<int> rowMap = extractIndices(logisticArray);

        for (int j = 0; j < height; j++)
        {
            int y = rowMap[j];
            int x = i;
            outputImage.at<cv::Vec3b>(j, i) = tempImage.at<cv::Vec3b>(y, x);
        }
    }
    return outputImage;
}
// PicEncrypt 行+列混淆解码
cv::Mat LG_RC_dec(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat tempImage(image.size(), image.type());
    cv::Mat outputImage(image.size(), image.type());

    double currentValue = stod(key);

    for (int i = 0; i < width; i++)
    {
        std::vector<std::pair<double, int>> logisticArray = logisticMap(currentValue, height);
        currentValue = logisticArray[height - 1].first;
        std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);
        std::vector<int> rowMap = extractIndices(logisticArray);

        for (int j = 0; j < height; j++)
        {
            int y = rowMap[j];
            int x = i;
            tempImage.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(j, i);
        }
    }

    currentValue = stod(key);
    for (int j = 0; j < height; j++)
    {
        std::vector<std::pair<double, int>> logisticArray = logisticMap(currentValue, width);
        currentValue = logisticArray[width - 1].first;
        std::sort(logisticArray.begin(), logisticArray.end(), logisticSort);
        std::vector<int> columnMap = extractIndices(logisticArray);

        for (int i = 0; i < width; i++)
        {
            int x = columnMap[i];
            int y = j;
            for (int c = 0; c < 3; c++)
                outputImage.at<cv::Vec3b>(y, x) = tempImage.at<cv::Vec3b>(j, i);
        }
    }

    return outputImage;
}