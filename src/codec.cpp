#include <codec.hpp>


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
// logistic 数组排序
bool logisticSort(const std::pair<double, int> &value1, const std::pair<double, int> &value2)
{
    return value1.first < value2.first;
}
// 根据 md5 打乱数组
std::vector<int> shuffleArray(int arrLength, std::string seed)
{
    std::vector<int> arr(arrLength);      // 创建一个大小为arrLength的vector
    std::iota(arr.begin(), arr.end(), 0); // 填充vector的元素为其索引值
    for (int i = arr.size() - 1; i > 0; i--)
    {
        int rand = std::stoi(md5(seed + std::to_string(i)).substr(0, 7), nullptr, 16) % (i + 1);
        std::swap(arr[rand], arr[i]);
    }
    return arr;
}
// 根据 key 生成 logistic 映射
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
// 生成 hilbert 映射
void hilbertMap(std::vector<std::pair<int, int>> &map, int width, int height)
{
    if (width >= height) {
        hilbertIterator(0, 0, width, 0, 0, height, map);
    } else {
        hilbertIterator(0, 0, 0, height, width, 0, map);
    }
}
//hilbert 迭代器
void hilbertIterator(int currentX, int currentY, int deltaX1, int deltaY1, int deltaX2, int deltaY2, std::vector<std::pair<int, int>>& map)
{
    auto sign = [](int value)
    {
        if (value == 0)
            return 0;
        if (value < 0)
            return -1;
        return 1;
    };
    
    int totalStepsX = std::abs(deltaX1 + deltaY1);
    int totalStepsY = std::abs(deltaX2 + deltaY2);

    int stepDirX1 = sign(deltaX1);
    int stepDirY1 = sign(deltaY1);
    int stepDirX2 = sign(deltaX2);
    int stepDirY2 = sign(deltaY2);

    if (totalStepsY == 1)
    {
        for (int step = 0; step < totalStepsX; step++)
        {
            map.push_back(std::pair<int, int>(currentX, currentY));
            currentX += stepDirX1;
            currentY += stepDirY1;
        }
        return;
    }
    if (totalStepsX == 1)
    {
        for (int step = 0; step < totalStepsY; step++)
        {
            map.push_back(std::pair<int, int>(currentX, currentY));
            currentX += stepDirX2;
            currentY += stepDirY2;
        }
        return;
    }
    
    int halfDeltaX1 = deltaX1 / 2;
    int halfDeltaY1 = deltaY1 / 2;
    int halfDeltaX2 = deltaX2 / 2;
    int halfDeltaY2 = deltaY2 / 2;

    int halfStepsX = std::abs(halfDeltaX1 + halfDeltaY1);
    int halfStepsY = std::abs(halfDeltaX2 + halfDeltaY2);

    if (2 * totalStepsX > 3 * totalStepsY)
    {
        if ((halfStepsX % 2) && (totalStepsX > 2))
        {
            // prefer even steps
            halfDeltaX1 += stepDirX1;
            halfDeltaY1 += stepDirY1;
        }
        // long case: split in two parts only
        hilbertIterator(currentX, currentY, halfDeltaX1, halfDeltaY1, deltaX2, deltaY2, map);
        hilbertIterator(currentX + halfDeltaX1, currentY + halfDeltaY1, deltaX1 - halfDeltaX1, deltaY1 - halfDeltaY1, deltaX2, deltaY2, map);
    }
    else
    {
        if ((halfStepsY % 2) && (totalStepsY > 2))
        {
            // prefer even steps
            halfDeltaX2 += stepDirX2;
            halfDeltaY2 += stepDirY2;
        }
        // standard case: one step up, one long horizontal, one step down
        hilbertIterator(currentX, currentY, halfDeltaX2, halfDeltaY2, halfDeltaX1, halfDeltaY1, map);
        hilbertIterator(currentX + halfDeltaX2, currentY + halfDeltaY2, deltaX1, deltaY1, halfDeltaX2 - stepDirX2, halfDeltaY2 - stepDirY2, map);
        hilbertIterator(currentX + (deltaX1 - stepDirX1) + (halfDeltaX2 - stepDirX2), currentY + (deltaY1 - stepDirY1) + (halfDeltaY2 - stepDirY2),
            -halfDeltaX2, -halfDeltaY2, -(deltaX1 - halfDeltaX1), -(deltaY1 - halfDeltaY1), map);
    }
}
//番茄混淆编码
cv::Mat encryptHBC(const cv::Mat &image)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(image.size(), image.type());

    std::vector<std::pair<int, int>> hilbertArray;
    hilbertMap(hilbertArray,width,height);

    int offset=std::round((std::sqrt(5)-1)/2*width*height);

    for(int k=0;k<width*height;k++)
    {
        int x = hilbertArray[k].first;
        int y = hilbertArray[k].second;
        int i=hilbertArray[(k + offset) % (width * height)].first;
        int j=hilbertArray[(k + offset) % (width * height)].second;
        outputImage.at<cv::Vec3b>(j, i) = image.at<cv::Vec3b>(y, x);
    }
    return outputImage;
}
//番茄混淆解码
cv::Mat decryptHBC(const cv::Mat &image)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage(image.size(), image.type());

    std::vector<std::pair<int, int>> hilbertArray;
    hilbertMap(hilbertArray,width,height);

    int offset=std::round((std::sqrt(5)-1)/2*width*height);

    for(int k=0;k<width*height;k++)
    {
        int x = hilbertArray[k].first;
        int y = hilbertArray[k].second;
        int i=hilbertArray[(k + offset) % (width * height)].first;
        int j=hilbertArray[(k + offset) % (width * height)].second;
        outputImage.at<cv::Vec3b>(y, x) = image.at<cv::Vec3b>(j, i);
    }
    return outputImage;
}
// 方块混淆编码
cv::Mat encryptMD5_B(const cv::Mat &image, const std::string key, int blockSizeX, int blockSizeY)
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

    std::vector<int> xMap = shuffleArray(blockSizeX, key);
    std::vector<int> yMap = shuffleArray(blockSizeY, key);

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
cv::Mat decryptMD5_B(const cv::Mat &image, const std::string key, int blockSizeX, int blockSizeY)
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
    
    std::vector<int> xMap = shuffleArray(blockSizeX, key);
    std::vector<int> yMap = shuffleArray(blockSizeY, key);
    
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
cv::Mat encryptMD5_P(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage(image.size(), image.type());

    std::vector<int> xMap = shuffleArray(width, key);
    std::vector<int> yMap = shuffleArray(height, key);

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
cv::Mat decryptMD5_P(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;

    cv::Mat outputImage(image.size(), image.type());

    // 打乱映射数组
    std::vector<int> xMap = shuffleArray(width, key);
    std::vector<int> yMap = shuffleArray(height, key);

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
cv::Mat encryptMD5_RP(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(image.size(), image.type());

    std::vector<int> xMap = shuffleArray(width, key);
    std::vector<int> yMap = shuffleArray(height, key);

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
cv::Mat decryptMD5_RP(const cv::Mat &image, const std::string &key)
{
    int width = image.cols;
    int height = image.rows;
    cv::Mat outputImage(image.size(), image.type());

    std::vector<int> xMap = shuffleArray(width, key);
    std::vector<int> yMap = shuffleArray(height, key);

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
cv::Mat encryptLG_R(const cv::Mat &image, const std::string &key)
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
//PicEncrypt 行混淆解码
cv::Mat decryptLG_R(const cv::Mat &image, const std::string &key)
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
//PicEncrypt 行+列混淆编码
cv::Mat encryptLG_RC(const cv::Mat &image, const std::string &key)
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
//PicEncrypt 行+列混淆解码
cv::Mat decryptLG_RC(const cv::Mat &image, const std::string &key)
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