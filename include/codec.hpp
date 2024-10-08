#ifndef CODEC_HPP
#define CODEC_HPP
#define OPENSSL_SUPPRESS_DEPRECATED

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <openssl/md5.h>
#include <cmath>

bool logisticSort(const std::pair<double, int> &value1, const std::pair<double, int> &value2);

std::string md5(const std::string &input);

void shuffleArray(std::vector<int> &arr, std::string seed);

std::vector<std::pair<double, int>> logisticMap(double key, int size);

std::vector<int> extractIndices(const std::vector<std::pair<double, int>> &pairs);

void hilbertMap(std::vector<std::pair<int, int>> &map, int width, int height);

void hilbertIterator(int x, int y, int ax, int ay, int bx, int by, std::vector<std::pair<int, int>> &map);

cv::Mat encryptHBC(const cv::Mat &image);

cv::Mat decryptHBC(const cv::Mat &image);

cv::Mat encryptMD5_B(const cv::Mat &image, const std::string key, int blockSizeX, int blockSizeY);

cv::Mat decryptMD5_B(const cv::Mat &image, const std::string key, int blockSizeX, int blockSizeY);

cv::Mat encryptMD5_P(const cv::Mat &image, const std::string &key);

cv::Mat decryptMD5_P(const cv::Mat &image, const std::string &key);

cv::Mat encryptMD5_RP(const cv::Mat &image, const std::string &key);

cv::Mat decryptMD5_RP(const cv::Mat &image, const std::string &key);

cv::Mat encryptLG_R(const cv::Mat &image, const std::string &key);

cv::Mat decryptLG_R(const cv::Mat &image, const std::string &key);

cv::Mat encryptLG_RC(const cv::Mat &image, const std::string &key);

cv::Mat decryptLG_RC(const cv::Mat &image, const std::string &key);

#endif // CODEC_HPP