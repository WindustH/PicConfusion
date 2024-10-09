#include "base.hpp"

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
// 根据 md5 打乱数组
std::vector<int> md5Shuffle(int arrLength, std::string seed)
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

// logistic 数组排序
bool logisticSort(const std::pair<double, int> &value1, const std::pair<double, int> &value2)
{
    return value1.first < value2.first;
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

//获取值的符号
int sign(int value)
{
    if (value == 0)
        return 0;
    if (value < 0)
        return -1;
    return 1;
};

void hilbertMap(std::vector<std::pair<int, int>> &map, int width, int height)
{
    map.clear();  // 清空vector
    
    if (width >= height)
    {
        // 直接传递目标坐标，而不是差值
        hilbertIterator(0, 0, width, 0, 0, height, map);
    }
    else
    {
        hilbertIterator(0, 0, 0, height, width, 0, map);
    }
}

// 修改后的hilbertIterator
void hilbertIterator(int x, int y, int ax, int ay, int bx, int by, std::vector<std::pair<int, int>> &map)
{
    int w = std::abs(ax + ay);  // 总宽度
    int h = std::abs(bx + by);  // 总高度

    int dax = sign(ax);  // 主方向单位向量
    int day = sign(ay);
    int dbx = sign(bx);  // 正交方向单位向量
    int dby = sign(by);

    if (h == 1)
    {
        // 简单行填充
        for (int i = 0; i < w; i++)
        {
            map.push_back({x, y});
            x += dax;
            y += day;
        }
        return;
    }

    if (w == 1)
    {
        // 简单列填充
        for (int i = 0; i < h; i++)
        {
            map.push_back({x, y});
            x += dbx;
            y += dby;
        }
        return;
    }

    int ax2 = ax / 2;
    int ay2 = ay / 2;
    int bx2 = bx / 2;
    int by2 = by / 2;

    int w2 = std::abs(ax2 + ay2);
    int h2 = std::abs(bx2 + by2);

    if (2 * w > 3 * h)
    {
        if ((w2 % 2) && (w > 2))
        {
            ax2 += dax;
            ay2 += day;
        }
        hilbertIterator(x, y, ax2, ay2, bx, by, map);
        hilbertIterator(x + ax2, y + ay2, ax - ax2, ay - ay2, bx, by, map);
    }
    else
    {
        if ((h2 % 2) && (h > 2))
        {
            bx2 += dbx;
            by2 += dby;
        }
        hilbertIterator(x, y, bx2, by2, ax2, ay2, map);
        hilbertIterator(x + bx2, y + by2, ax, ay, bx - bx2, by - by2, map);
        hilbertIterator(x + (ax - dax) + (bx2 - dbx), 
                       y + (ay - day) + (by2 - dby),
                       -bx2, -by2, 
                       -(ax - ax2), -(ay - ay2), map);
    }
}
