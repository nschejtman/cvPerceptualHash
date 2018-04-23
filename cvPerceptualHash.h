#ifndef CVPERCEPTUALHASH_LIBRARY_H
#define CVPERCEPTUALHASH_LIBRARY_H

#include <opencv2/opencv.hpp>

namespace cvPerceptualHash {
    /**
     * Calculates a 32-bit perceptual hash from an image. Perceptual hashing obtains close hashes for similar images
     * @param image openCV image for hashing
     */
    float hash(cv::Mat image);

    /**
     * Compares two images based on their perceptual hash difference
     * @param image1
     * @param image2
     * @return similarity percentage
     */
    float compareImgs(cv::Mat image1, cv::Mat image2);

    /**
     * Calculates the similarity percentage between two hashes
     * @param hash1
     * @param hash2
     * @return similarity percentage
     */
    float compareHashes(float hash1, float hash2);
}
#endif