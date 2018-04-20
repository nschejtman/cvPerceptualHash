#include <opencv2/opencv.hpp>
#include "cvPerceptualHash.h"

using namespace cv;

int main(int argc, char **argv) {
    Mat img1 = imread("/Users/nicolas/Pictures/profile.jpg");
    Mat img2 = imread("/Users/nicolas/Pictures/profilea.jpg");
    Mat img3 = imread("/Users/nicolas/Pictures/profile2.jpg");
    Mat img4 = imread("/Users/nicolas/Pictures/fotocv.jpg");
    return 0;

}