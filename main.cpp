#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <opencv/cv.hpp>

using namespace cv;
using namespace std;

// distance between point 1 and point 2
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


// exchange black to white and white to black and all gray levels in the middle
void negativeImage(Mat& image) {
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            image.at<float>(i, j) = 1 - image.at<float>(i, j);
        }
    }
}

int main( int argc, char** argv ) {
    Mat original;

    // Read the file
    original = imread("../PandaOriginal.bmp", CV_LOAD_IMAGE_COLOR);


    if(!original.data ){                              // Check for invalid input
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }


    namedWindow("ORIGINAL", CV_WINDOW_AUTOSIZE);// Create a window for display.

    imshow("ORIGINAL", original);


    // put each image next to each other
    moveWindow("ORIGINAL", 0, 0);            // put window in certain position in the screen

    //infinite loop
    while (1) {
        // if pressed ESC it closes the program
        if (waitKeyEx(10) == 27) {
            return 0;
        }
    }
}