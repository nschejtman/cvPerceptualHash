#include "cvPerceptualHash.h"
#include "MathUtils.cpp"

using namespace cv;

/**
 * This function transforms a BGR image to gray scale via desaturation
 * @param inputImage
 * @return gray scale image 1 channel image
 */
void flatten(const Mat &inputImage, Mat &outputImage) {
    Mat hsv, desaturated3C;
    cvtColor(inputImage, hsv, CV_BGR2HSV);                          // convert the inputImage to HSV space
    cvtColor(hsv & Vec3b(255, 0, 255), desaturated3C, CV_HSV2BGR);  // zero out the saturation channel

    Mat chans[3];
    split(desaturated3C, chans);                                    // extract each individual channel
    chans[0].copyTo(outputImage);
}

/**
 * Downsamples an input image by iteratively halving it's size until either its width matches hDim or its height matches
 * vDim
 * @param inputImage
 * @param outputImage
 * @param hDim
 * @param vDim
 */
void downsample(const Mat &inputImage, Mat &outputImage, int hDim, int vDim) {
    int originalWidth = inputImage.size().width;
    int originalHeight = inputImage.size().height;

    // Param checking
    if (originalWidth < hDim || originalHeight < vDim) {
        throw ("Cannot downsample! Target dimensions are larger than the actual image size");
    }
    if (!isPowerOfTwo(originalWidth) || !isPowerOfTwo(originalHeight)) {
        throw ("Image dimensions are not powers of two!");
    }
    if (!isPowerOfTwo(hDim), !isPowerOfTwo(vDim)) {
        throw ("Target dimensions are not powers of two!");
    }

    Mat aux, downsampled;
    inputImage.copyTo(aux);

    while (aux.size().width > hDim && aux.size().height > vDim) {
        pyrDown(aux, downsampled, Size(aux.size().width / 2, aux.size().height / 2));
        downsampled.copyTo(aux);
    }

    downsampled.copyTo(outputImage);
}

/**
 * Upsamples an input image by iteratively duplicating it's size until either its width matches hDim or its height
 * matches vDim
 * @param inputImage
 * @param outputImage
 * @param hDim
 * @param vDim
 */
void upsample(const Mat &inputImage, Mat &outputImage, int hDim, int vDim) {
    int originalWidth = inputImage.size().width;
    int originalHeight = inputImage.size().height;

    // Param checking
    if (originalWidth > hDim || originalHeight > vDim) {
        throw ("Cannot downsample! Target dimensions are smaller than the actual image size");
    }
    if (!isPowerOfTwo(originalWidth) || !isPowerOfTwo(originalHeight)) {
        throw ("Image dimensions are not powers of two!");
    }
    if (!isPowerOfTwo(hDim), !isPowerOfTwo(vDim)) {
        throw ("Target dimensions are not powers of two!");
    }

    Mat aux, upsampled;
    inputImage.copyTo(aux);

    while (aux.size().width < hDim && aux.size().height < vDim) {
        pyrUp(aux, upsampled, Size(aux.size().width * 2, aux.size().height * 2));
        upsampled.copyTo(aux);
    }

    upsampled.copyTo(outputImage);
}

/**
 * This function transforms the input image into a 256x256 squared image by resizing and downsampling
 * @param inputImage
 * @return 256x256 image
 */
void normalize(const Mat &inputImage, Mat &outputImage) {
    Mat resizedImage;

    // Resize the inputImage to an 2^{n} x 2^{n} squared inputImage
    int width = inputImage.size().width;
    int height = inputImage.size().height;
    int xDim = nearest2nValue(width), yDim = nearest2nValue(height);    // find the closest 2^{n} value
    int dim = (abs(width - xDim) < abs(height - yDim)) ? xDim : yDim;   // set the resize dimension
    resize(inputImage, resizedImage, Size(dim, dim));                   // resize the inputImage


    if (dim == 256) {
        resizedImage.copyTo(outputImage);
    } else if (dim > 256) { // downsample
        downsample(resizedImage, outputImage, 256, 256);
    } else { // upsample
        upsample(resizedImage, outputImage, 256, 256);
    }
}

/**
 * Get the top left n-th portion of the input image
 * @param inputImage
 * @param outputImage
 * @param n
 */
void cropTopLeftNthPortion(Mat &inputImage, Mat &outputImage, int n) {
    int x = inputImage.cols / n, y = inputImage.rows / 16;
    Mat croppedImage = Mat(inputImage, Rect(Point(0, 0), Point(x, y))).clone();
    croppedImage.copyTo(outputImage);
}

/**
 * Transfoms the input image from the spatial domain to the frequency domain via DCT (Discrete Cosine Transform)
 * @param inputImage
 * @param outputMat
 */
void transformToFreqSpace(const Mat &inputImage, Mat &outputMat) {
    Mat floatMat;
    if (inputImage.type() == CV_32F) {
        inputImage.copyTo(floatMat);
    } else {
        inputImage.convertTo(floatMat, CV_32F);
    }
    dct(floatMat, outputMat);
}

/**
 * Takes an input Mat corresponding to a DCT(?) transform and averages all its AC coefficients (ignores the
 * DC coefficient)
 * @param freqMat
 * @return
 */
float averageAcCoefs(const Mat &freqMat) {
    // Format matrix
    Mat _32bitMat;
    if (freqMat.type() == CV_32F) {
        freqMat.copyTo(_32bitMat);
    } else {
        freqMat.convertTo(_32bitMat, CV_32F);
    }

    // Get the total sum
    Mat rowSums, totalSum;
    reduce(_32bitMat, rowSums, 1, CV_REDUCE_SUM);
    reduce(rowSums, totalSum, 0, CV_REDUCE_SUM);

    float acCoefsSum = totalSum.at<float>(0, 0) - freqMat.at<float>(0, 0);      // subtract DC coefficient
    auto nAcCoefs = (float) (freqMat.rows * freqMat.cols - 1);                  // get the number of AC coefficients
    return acCoefsSum / nAcCoefs;
}

// Documented in .h
float cvPerceptualHash::hash(cv::Mat image) {
    Mat grayscale, normalized, transformed, cropped;
    flatten(image, grayscale);
    normalize(grayscale, normalized);
    transformToFreqSpace(normalized, transformed);
    cropTopLeftNthPortion(transformed, cropped, 16);
    float avg = averageAcCoefs(cropped);
    return avg;
}

// Documented in .h
float cvPerceptualHash::compareImgs(cv::Mat image1, cv::Mat image2) {
    float h1 = cvPerceptualHash::hash(image1);
    float h2 = cvPerceptualHash::hash(image2);

    float hashLength = sizeof(float) * 8;
    float hammingDist = hammingDistance(h1, h2);
    float similarityScore = (hashLength - hammingDist) / hashLength;
    return similarityScore;
}