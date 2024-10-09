#ifndef BASE_HPP
#define BASE_HPP
#define OPENSSL_SUPPRESS_DEPRECATED

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <openssl/md5.h>
#include <cmath>

std::string md5(const std::string &input);
std::vector<int> md5Shuffle(int arrLength, std::string seed);
bool logisticSort(const std::pair<double, int> &value1, const std::pair<double, int> &value2);
std::vector<std::pair<double, int>> logisticMap(double key, int size);
std::vector<int> extractIndices(const std::vector<std::pair<double, int>> &pairs);
void hilbertMap(std::vector<std::pair<int, int>> &map, int width, int height);
void hilbertIterator(int currentX, int currentY, int deltaX1, int deltaY1, int deltaX2, int deltaY2, std::vector<std::pair<int, int>> &map);

#endif // BASE_HPP