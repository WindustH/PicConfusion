#ifndef CODEC_HPP
#define CODEC_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

bool logisticSort(const std::pair<double, int> &value1, const std::pair<double, int> &value2);

std::string md5(const std::string &input);

void shuffleArray(std::vector<int> &arr, std::string seed);

std::vector<std::pair<double, int>> logisticMap(double key, int size);

std::vector<int> extractIndices(const std::vector<std::pair<double, int>> &pairs);

cv::Mat encryptBC(const cv::Mat &image, const std::string key, int blockSizeX, int blockSizeY);

cv::Mat decryptBC(const cv::Mat &image, const std::string key, int blockSizeX, int blockSizeY);

cv::Mat encryptPC(const cv::Mat &image, const std::string &key);

cv::Mat decryptPC(const cv::Mat &image, const std::string &key);

cv::Mat encryptRPC(const cv::Mat &image, const std::string &key);

cv::Mat decryptRPC(const cv::Mat &image, const std::string &key);

cv::Mat encryptPER(const cv::Mat &image, const std::string &key);

cv::Mat decryptPER(const cv::Mat &image, const std::string &key);

cv::Mat encryptPERC(const cv::Mat &image, const std::string &key);

cv::Mat decryptPERC(const cv::Mat &image, const std::string &key);

#endif // CODEC_HPP