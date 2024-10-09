#ifndef CODEC_HPP
#define CODEC_HPP

#include <opencv2/opencv.hpp>

cv::Mat HBC_enc(const cv::Mat &image);
cv::Mat HBC_dec(const cv::Mat &image);
cv::Mat MD5_B_enc(const cv::Mat &image, const std::string &key, const int &blockSizeX, const int &blockSizeY);
cv::Mat MD5_B_dec(const cv::Mat &image, const std::string &key, const int &blockSizeX, const int &blockSizeY);
cv::Mat MD5_P_enc(const cv::Mat &image, const std::string &key);
cv::Mat MD5_P_dec(const cv::Mat &image, const std::string &key);
cv::Mat MD5_RP_enc(const cv::Mat &image, const std::string &key);
cv::Mat MD5_RP_dec(const cv::Mat &image, const std::string &key);
cv::Mat LG_R_enc(const cv::Mat &image, const std::string &key);
cv::Mat LG_R_dec(const cv::Mat &image, const std::string &key);
cv::Mat LG_RC_enc(const cv::Mat &image, const std::string &key);
cv::Mat LG_RC_dec(const cv::Mat &image, const std::string &key);

#endif // CODEC_HPP