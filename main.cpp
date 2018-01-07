
#include <iostream>
#include <cmath>
#include <vector>
#include "include/Net.h"
#include "include/TrainingData.h"
#import <ApplicationServices/ApplicationServices.h>


#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include <opencv2/core/types.hpp>
#include <sstream>
#include "dirent.h"
#include <regex>

using namespace std;
using namespace cv;

enum {
    kVK_ANSI_A                    = 0x00,
    kVK_ANSI_S                    = 0x01,
    kVK_ANSI_D                    = 0x02,
    kVK_ANSI_F                    = 0x03,
    kVK_ANSI_H                    = 0x04,
    kVK_ANSI_G                    = 0x05,
    kVK_ANSI_Z                    = 0x06,
    kVK_ANSI_X                    = 0x07,
    kVK_ANSI_C                    = 0x08,
    kVK_ANSI_V                    = 0x09,
    kVK_ANSI_B                    = 0x0B,
    kVK_ANSI_Q                    = 0x0C,
    kVK_ANSI_W                    = 0x0D,
    kVK_ANSI_E                    = 0x0E,
    kVK_ANSI_R                    = 0x0F,
    kVK_ANSI_Y                    = 0x10,
    kVK_ANSI_T                    = 0x11,
    kVK_ANSI_1                    = 0x12,
    kVK_ANSI_2                    = 0x13,
    kVK_ANSI_3                    = 0x14,
    kVK_ANSI_4                    = 0x15,
    kVK_ANSI_6                    = 0x16,
    kVK_ANSI_5                    = 0x17,
    kVK_ANSI_Equal                = 0x18,
    kVK_ANSI_9                    = 0x19,
    kVK_ANSI_7                    = 0x1A,
    kVK_ANSI_Minus                = 0x1B,
    kVK_ANSI_8                    = 0x1C,
    kVK_ANSI_0                    = 0x1D,
    kVK_ANSI_RightBracket         = 0x1E,
    kVK_ANSI_O                    = 0x1F,
    kVK_ANSI_U                    = 0x20,
    kVK_ANSI_LeftBracket          = 0x21,
    kVK_ANSI_I                    = 0x22,
    kVK_ANSI_P                    = 0x23,
    kVK_ANSI_L                    = 0x25,
    kVK_ANSI_J                    = 0x26,
    kVK_ANSI_Quote                = 0x27,
    kVK_ANSI_K                    = 0x28,
    kVK_ANSI_Semicolon            = 0x29,
    kVK_ANSI_Backslash            = 0x2A,
    kVK_ANSI_Comma                = 0x2B,
    kVK_ANSI_Slash                = 0x2C,
    kVK_ANSI_N                    = 0x2D,
    kVK_ANSI_M                    = 0x2E,
    kVK_ANSI_Period               = 0x2F,
    kVK_ANSI_Grave                = 0x32,
    kVK_ANSI_KeypadDecimal        = 0x41,
    kVK_ANSI_KeypadMultiply       = 0x43,
    kVK_ANSI_KeypadPlus           = 0x45,
    kVK_ANSI_KeypadClear          = 0x47,
    kVK_ANSI_KeypadDivide         = 0x4B,
    kVK_ANSI_KeypadEnter          = 0x4C,
    kVK_ANSI_KeypadMinus          = 0x4E,
    kVK_ANSI_KeypadEquals         = 0x51,
    kVK_ANSI_Keypad0              = 0x52,
    kVK_ANSI_Keypad1              = 0x53,
    kVK_ANSI_Keypad2              = 0x54,
    kVK_ANSI_Keypad3              = 0x55,
    kVK_ANSI_Keypad4              = 0x56,
    kVK_ANSI_Keypad5              = 0x57,
    kVK_ANSI_Keypad6              = 0x58,
    kVK_ANSI_Keypad7              = 0x59,
    kVK_ANSI_Keypad8              = 0x5B,
    kVK_ANSI_Keypad9              = 0x5C
};

/* keycodes for keys that are independent of keyboard layout*/
enum {
    kVK_Return                    = 0x24,
    kVK_Tab                       = 0x30,
    kVK_Space                     = 0x31,
    kVK_Delete                    = 0x33,
    kVK_Escape                    = 0x35,
    kVK_Command                   = 0x37,
    kVK_Shift                     = 0x38,
    kVK_CapsLock                  = 0x39,
    kVK_Option                    = 0x3A,
    kVK_Control                   = 0x3B,
    kVK_RightCommand              = 0x36,
    kVK_RightShift                = 0x3C,
    kVK_RightOption               = 0x3D,
    kVK_RightControl              = 0x3E,
    kVK_Function                  = 0x3F,
    kVK_F17                       = 0x40,
    kVK_VolumeUp                  = 0x48,
    kVK_VolumeDown                = 0x49,
    kVK_Mute                      = 0x4A,
    kVK_F18                       = 0x4F,
    kVK_F19                       = 0x50,
    kVK_F20                       = 0x5A,
    kVK_F5                        = 0x60,
    kVK_F6                        = 0x61,
    kVK_F7                        = 0x62,
    kVK_F3                        = 0x63,
    kVK_F8                        = 0x64,
    kVK_F9                        = 0x65,
    kVK_F11                       = 0x67,
    kVK_F13                       = 0x69,
    kVK_F16                       = 0x6A,
    kVK_F14                       = 0x6B,
    kVK_F10                       = 0x6D,
    kVK_F12                       = 0x6F,
    kVK_F15                       = 0x71,
    kVK_Help                      = 0x72,
    kVK_Home                      = 0x73,
    kVK_PageUp                    = 0x74,
    kVK_ForwardDelete             = 0x75,
    kVK_F4                        = 0x76,
    kVK_End                       = 0x77,
    kVK_F2                        = 0x78,
    kVK_PageDown                  = 0x79,
    kVK_F1                        = 0x7A,
    kVK_LeftArrow                 = 0x7B,
    kVK_RightArrow                = 0x7C,
    kVK_DownArrow                 = 0x7D,
    kVK_UpArrow                   = 0x7E
};

// distance between point 1 and point 2
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

float averageErrorBnW(Mat& originalImage, Mat& image){

    float error = 0;
    for (int r = 0; r < image.rows; r++) {
        for (int c = 0; c < image.cols; c++) {
            float tmp = abs(originalImage.at<uchar>(r,c) - image.at<uchar>(r,c));
            error += tmp*tmp;
        }
    }

    error /= image.rows*image.cols;

    return error;
}

void showVector(string label, vector<double> &v){
    cout << label << " ";
    for (int i = 0; i < v.size(); i++){
        cout << v[i] << " ";
    }
    cout << endl;
}

void pressKey(CGKeyCode keyCode){
    CGEventRef keyup, keydown;
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    keydown = CGEventCreateKeyboardEvent (source, keyCode, true);
    keyup = CGEventCreateKeyboardEvent (source, keyCode, false);

    //CGEventSetFlags(keydown, kCGEventFlagMaskCommand);  // this adds CTRL

    CGEventPost(kCGHIDEventTap, keydown);
    CGEventPost(kCGHIDEventTap, keyup);

    //CGEventPost(kCGSessionEventTap, keydown);
    //CGEventPost(kCGSessionEventTap, keyup);

    //CGEventPost(kCGAnnotatedSessionEventTap, saveCommandDown);
    //CGEventPost(kCGAnnotatedSessionEventTap, saveCommandUp);

    CFRelease(keydown);
    CFRelease(keyup);
    CFRelease(source);
}


void pressButton(int i) {
    sleep(1);
    cout << "Starting" << endl;
    CGKeyCode button;
    switch (i){
        case 0:
            button = kVK_ANSI_Q;
            break;
        case 1:
            button = kVK_DownArrow;
            break;
        case 2:
            button = kVK_UpArrow;
            break;
        case 3:
            button = kVK_LeftArrow;
            break;
        case 4:
            button = kVK_RightArrow;
            break;
        default:

            break;
    }

    pressKey(button);
}


Mat getScreen(){
    CGDisplayCount displayCount;
    CGDirectDisplayID displays[32];

    // grab the active displays
    CGGetActiveDisplayList(32, displays, &displayCount);

    CGRect frame = CGRectMake(0, 109, 640, 400);

    CGImageRef CGImage = CGDisplayCreateImageForRect(displays[0], frame);


    if (CGImage == NULL){
        cout << "CGImage did not read the display successfully." << endl;
    }



    // make a snapshot of the current display
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(CGImage);
    CGFloat cols = CGImageGetWidth(CGImage)/2;
    CGFloat rows = CGImageGetHeight(CGImage)/2;
    Mat image = Mat(rows, cols, CV_8UC4);


    CGContextRef contextRef = CGBitmapContextCreate(image.data,                 // Pointer to backing data
                                                    cols,                      // Width of bitmap
                                                    rows,                     // Height of bitmap
                                                    8,                          // Bits per component
                                                    image.step[0],              // Bytes per row
                                                    colorSpace,                 // Colorspace
                                                    kCGImageAlphaNoneSkipLast |
                                                    kCGBitmapByteOrderDefault); // Bitmap info flags

    CGContextDrawImage(contextRef, CGRectMake(0, 0, cols, rows), CGImage);
    CGContextRelease(contextRef);
    CGImageRelease(CGImage);
    cvtColor(image, image, CV_RGB2BGR);

    imshow("ORIGINAL", image);
    moveWindow("ORIGINAL", 650, 0);


    return image;
}

// function created to go through the screen plane 2 (Blue) and save manually selected blocks
// Press ESC to save that block or any other button to continue without saving
void goThroughScreenAndSave(int height, int width, int tileHeight, int tileWidth, Mat boarder1, vector<Mat> planes){
    for (int i = 0; i < (height / tileHeight) * tileHeight; i += tileHeight) {
        for (int j = 0; j < (width / tileWidth) * tileWidth - tileWidth; j++) {
            //get 8x8 block from image from (j,i) coordinates
            Mat block;
            if (j == 0) block = boarder1(cv::Rect(j, i, tileWidth, tileHeight));
            else block = planes[2](cv::Rect(j, i, tileWidth, tileHeight));

            //stringstream pictureName;
            //pictureName << "Ghost1.bmp";
            stringstream pictureName;
            pictureName << "Tile" << j << "-" << i << ".bmp";
            cout << pictureName.str() << endl;
            imshow("Block", block);
            if (waitKeyEx(25000) == 27) {
                imwrite(pictureName.str(), block);
            }
            //cout << "Erms: " << averageErrorBnW(block, block) << block << endl;
        }
    }
}

Mat getTiles(Mat screen, vector<Mat> sprites) {
    int height = screen.rows;
    int width = screen.cols;
    int tileHeight = 24;
    int tileWidth = 32;

    // split in 3 planes RGB
    vector<Mat> planes;
    split(screen, planes);

    int planeToCheck = 2;

    Mat tiles = Mat(height / tileHeight, width / tileWidth, CV_32F);

    Mat boarder1 = planes[planeToCheck](cv::Rect(0, 0, tileWidth, height));

    imshow("Plane", planes[planeToCheck]);
    moveWindow("Plane", 650, 500);

    //goThroughScreenAndSave(height, width, tileHeight, tileWidth, boarder1, planes);

    for (int i = 0; i < (height / tileHeight) * tileHeight; i += tileHeight) {
        for (int j = 0; j < (width / tileWidth) * tileWidth; j += tileWidth) {
            //get 8x8 block from image from (j,i) coordinates
            Mat block;
            if (j == 0) block = boarder1(cv::Rect(j, i, tileWidth, tileHeight));
            else block = planes[planeToCheck](cv::Rect(j, i, tileWidth, tileHeight));


            bool spriteFound = false;
            int mostSimilar = -1;
            int aeBnW = 500000000;
            for (int k = 0; k < sprites.size(); k++) {


                if (averageErrorBnW(sprites[k], block) < aeBnW) {
                    mostSimilar = k;
                    aeBnW = averageErrorBnW(sprites[k], block);
                    if (averageErrorBnW(sprites[k], block) < 500) {
                        spriteFound = true;
                    }
                }
                if (spriteFound || (k + 1) == sprites.size()) {

                    if (mostSimilar < 0 || !spriteFound && aeBnW > 1000) {
                        cout << "No sprite found!" << endl;
                        cout << "AVERAGE ERROR" << aeBnW << "  :  " << mostSimilar << endl;
                        cout << block << endl;
                        cout << sprites[mostSimilar] << endl;
                        imshow("BLOCK", block);
                        waitKeyEx(25000);
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 8796;
                    }
                    else if (mostSimilar < 2) tiles.at<float>(i / tileHeight, j / tileWidth) = 0;   //Blank
                    else if (mostSimilar >= 2 && mostSimilar < 7)
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.15; // Ghosts
                    else if (mostSimilar == 7) tiles.at<float>(i / tileHeight, j / tileWidth) = 0.3; // player
                    else if (mostSimilar >= 8 && mostSimilar < 10)
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.45; // points
                    else if (mostSimilar >= 10 && mostSimilar < 12)
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.6; // ExtraPoints
                    else if (mostSimilar >= 12 && mostSimilar < 14)
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.75; // ScaryGhosts
                    else if (mostSimilar >= 14) tiles.at<float>(i / tileHeight, j / tileWidth) = 1;  // Walls
                    continue;
                }
            }

            //stringstream pictureName;
            //pictureName << "Tile" << j << "-" << i << ".bmp";
            //cout << pictureName.str() << endl;
            //imshow("Block", block);
            //imwrite(pictureName.str(), block);
            //cout << "Erms: " << averageErrorBnW(block, block) << block << endl;
            //waitKeyEx(25000);
        }
    }

    return tiles;
}

void getSprites(vector<Mat>& sprites, char* location){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (location)) != NULL) {

        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {

            cout << regex_match (ent->d_name, regex("\\.(.*)")) << endl;
            if (regex_match (ent->d_name, regex("\\.(.*)"))){
                cout << "File beggining with . is not accepted:" << ent->d_name << endl;
                continue;
            }
            char* subLocation = new char[strlen(location) + 1 + strlen(ent->d_name)+1];

            strcpy(subLocation, location);
            strcat(subLocation, ent->d_name);
            strcat(subLocation, "/");
            getSprites(sprites, subLocation);
            delete [] subLocation;

        }
        closedir (dir);

    } else {
        cout << "Directory does not exist!" << endl;
        location[strlen(location)-1] = 0;
        Mat sprite = imread(location, CV_LOAD_IMAGE_GRAYSCALE);
        if (!sprite.data) {
            cout << "Could not open or find the image in: " << location << endl;
        }
        else {
            cout << "Saving sprite: " << location << endl;
            sprites.push_back(sprite);
        }
    }
}

Mat scaleUp(Mat image, int scale){
    resize(image, image, image.size()*scale, scale, scale);   //resize image
    imshow("Tiles3", image);                   // Show our image inside it.
    moveWindow("Tiles3", 650, 300);
    return image;
}

/*
226 pared y puntos
209-208 jugador
 214 fantasma asustado a punto de aparecer
198 fantasma rojo
 196 fantasma rosa
 179 fantasma naranja
 88 fantasma verde
68 fantasma asustado
 182 cerecas
 */

// size 21x14

int main( int argc, char** argv ) {
    vector<Mat> sprites;
    char* startLocation = "../Images/";
    getSprites(sprites, startLocation);

    sleep(2);
    //infinite Exit loop
    while (1) {
        Mat screen = getScreen();

        Mat tiles = getTiles(screen, sprites);
        cout << tiles << endl;
        scaleUp(tiles, 20);

        // if pressed ESC it closes the program
        if (waitKeyEx(25000) == 27) {
            return 0;
        }
    }


    int x;
    cin >> x;


    srand(time(nullptr));
    //srand(0);

    TrainingData trainData;

    vector<double> topology;
    trainData.getTopology(topology);


    Net myNet(topology);



    vector<double> inputs;
    vector<double> targets;
    vector<double> results;


    for (int i = 0; i < 1000; i++){
        cout << endl << "Pass "<< i;

        // Get new inputs data and feed it forward;
        inputs.clear();
        if (trainData.getNextInputs(inputs) != topology[0]){
            break;
        }

        showVector(": Inputs:", inputs);
        myNet.feedForward(inputs);


        //Collect the net's actual results
        myNet.getResults(results);
        showVector("Outputs:", results);


        //Train the net what the ouputs should have been
        trainData.getTargetOutputs(targets);
        showVector("Targets:", targets);

        assert(targets.size() == topology.back());

        myNet.backProp(targets);

        //Report how well the training is working. averaged over recent...
        cout << "Net recent average error: " << myNet.getRecentAverageError() << endl;


/*
        if (results[0] <- 0.5)                      pressButton(1);
        if (results[0] < 0 && results[0] >= -0.5)   pressButton(2);
        if (results[0] < 0.5 && results[0] >= 0)    pressButton(3);
        if (results[0] < 1 && results[0] >= 0.5)    pressButton(4);
*/

    }

    cout << "DONE" << endl;
	putchar(getchar());
}