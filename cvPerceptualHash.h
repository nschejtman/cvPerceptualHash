#ifndef CVPERCEPTUALHASH_LIBRARY_H
#define CVPERCEPTUALHASH_LIBRARY_H

#include <opencv2/opencv.hpp>

namespace cvPerceptualHash {
/**
 *
 * @param image openCV image for hashing
 * @param hashLength length of the hash in bytes
 * @param outputBuffer buffer where the resulting hash will be stored
 */
    float hash(cv::Mat image);

    float compareImgs(cv::Mat image1, cv::Mat image2);
}
#endif