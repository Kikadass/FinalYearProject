
#include <iostream>
#include <cmath>
#include <vector>
#include "include/TrainingData.h"
#import <ApplicationServices/ApplicationServices.h>
#include "nlohmann/json.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include <opencv2/core/types.hpp>
#include <sstream>
#include "dirent.h"
#include "include/Pool.h"
#include <regex>
#include "gnuplot-cpp/gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

using namespace std;
using namespace cv;

using json = nlohmann::json;

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

//create a map of the tiles by the averages of the colors in blocks
Mat getAveragesColor(Mat screen) {
    int height = screen.rows;
    int width = screen.cols;
    int tileHeight = 24;
    int tileWidth = 32;

    Mat tiles = Mat(height / tileHeight, width / tileWidth, CV_32S);

    for (int i = 0; i < (height / tileHeight) * tileHeight; i += tileHeight) {
        for (int j = 0; j < (width / tileWidth) * tileWidth; j += tileWidth) {

            //get the sum of all the colors of the block in the 3 planes in order to get the average
            float color = 0;
            for (int k = 0; k < 3; k++) {
                //get 8x8 block from image from (j,i) coordinates
                Mat block;
                block = screen(cv::Rect(j, i, tileWidth, tileHeight));

                for (int x = 0; x < block.rows; x++){
                    for (int y = 0; y < block.cols; y++){
                        color += block.at<Vec3b>(x,y)[k];
                    }
                }
            }

            color /= (3.0f*(float)tileWidth*(float)tileHeight);
            tiles.at<int>(i / tileHeight, j / tileWidth) = (int)color;
        }
    }

    return tiles;
}

Mat getAveragesBnW(Mat screen) {
    int height = screen.rows;
    int width = screen.cols;
    int tileHeight = 24;
    int tileWidth = 32;
    int planeSelected = 1;

    Mat tiles = Mat(height / tileHeight, width / tileWidth, CV_32S);

    for (int i = 0; i < (height / tileHeight) * tileHeight; i += tileHeight) {
        for (int j = 0; j < (width / tileWidth) * tileWidth; j += tileWidth) {

            //get the sum of all the colors of the block in the 3 planes in order to get the average
            float color = 0;
            //get 8x8 block from image from (j,i) coordinates
            Mat block;
            block = screen(cv::Rect(j, i, tileWidth, tileHeight));

            for (int x = 0; x < block.rows; x++) {
                for (int y = 0; y < block.cols; y++) {
                    color += block.at<Vec3b>(x, y)[planeSelected];
                }
            }


            color /= ((float) tileWidth * (float) tileHeight);
            tiles.at<int>(i / tileHeight, j / tileWidth) = (int) color;
        }
    }

    return tiles;
}

void pressKey(CGKeyCode keyCode){
    CGEventRef keyup, keydown;
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    keydown = CGEventCreateKeyboardEvent (source, keyCode, true);
    keyup = CGEventCreateKeyboardEvent (source, keyCode, false);

    CGEventPost(kCGHIDEventTap, keydown);

    //press button for 50ms
    usleep(100000);
    CGEventPost(kCGHIDEventTap, keyup);


    CFRelease(keydown);
    CFRelease(keyup);
    CFRelease(source);
}

void pressButton(int i) {
    CGKeyCode button;
    switch (i){
        case 1:
            button = kVK_DownArrow;
            cout << "Pressing Down" << endl;
            break;
        case 2:
            button = kVK_UpArrow;
            cout << "Pressing Up" << endl;
            break;
        case 3:
            button = kVK_LeftArrow;
            cout << "Pressing Left" << endl;
            break;
        case 4:
            button = kVK_RightArrow;
            cout << "Pressing Right" << endl;
            break;
        case 5:
            button = kVK_Space;
            cout << "Pressing Space" << endl;
            break;
        case 6:
            button = kVK_Return;
            cout << "Pressing Return" << endl;
            break;
        case 7:
            button = kVK_F10;
            cout << "Pressing Load State" << endl;
            break;
        default:
            cout << "ERROR: Wrong Button pressed!  -> " << i << endl;
            return;
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

    //imshow("ORIGINAL", image);
    //moveWindow("ORIGINAL", 650, 0);


    return image;
}

// function created to go through the screen plane 2 (Blue) and save manually selected blocks
// Press ESC to save that block or any other button to continue without saving
void goThroughScreenAndSave(int height, int width, int tileHeight, int tileWidth, vector<Mat> planes){
    for (int i = 15*tileHeight+1; i < (height / tileHeight) * tileHeight; i += tileHeight) {
        for (int j = (width / tileWidth) * tileWidth - tileWidth*8-6; j > 0 ; j -= tileWidth) {
            //get 8x8 block from image from (j,i) coordinates
            Mat block = planes[2](cv::Rect(j, i, tileWidth, tileHeight));

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

Mat getTiles(Mat screen, vector<Mat> sprites, int& originalNPoints, int& points) {
    int height = screen.rows;
    int width = screen.cols;
    int tileHeight = 24;
    int tileWidth = 32;
    int nPoints = 0;

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
                        /*
                        cout << "No sprite found!" << endl;
                        cout << "AVERAGE ERROR" << aeBnW << "  :  " << mostSimilar << endl;
                        cout << block << endl;
                        cout << sprites[mostSimilar] << endl;
                        imshow("BLOCK", block);
                        waitKeyEx(25000);
                         */

                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.9;
                    }
                    else if (mostSimilar < 2) tiles.at<float>(i / tileHeight, j / tileWidth) = 0;   //Blank
                    else if (mostSimilar >= 2 && mostSimilar < 7)
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.15; // Ghosts
                    else if (mostSimilar == 7) tiles.at<float>(i / tileHeight, j / tileWidth) = 0.3; // player
                    else if (mostSimilar >= 8 && mostSimilar < 10) {
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.45; // points
                        nPoints++;
                    }
                    else if (mostSimilar >= 10 && mostSimilar < 12) {
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.6; // ExtraPoints
                        nPoints += 5;
                    }
                    else if (mostSimilar >= 12 && mostSimilar < 14)
                        tiles.at<float>(i / tileHeight, j / tileWidth) = 0.75; // ScaryGhosts
                    else if (mostSimilar >= 14) tiles.at<float>(i / tileHeight, j / tileWidth) = 1;  // Walls
                    continue;
                }
            }
        }
    }

    if (originalNPoints == -1) originalNPoints = nPoints;

    points = originalNPoints - nPoints;

    return tiles;
}

bool checkScoreFile(string scoresFileName, string tilesFileName){
    ifstream scoresFile;
    ifstream tilesFile;
    int maxTime = 10*CLOCKS_PER_SEC; //10s
    clock_t start = clock();
    string line = "";

    bool scoresFound = false;

    do {
        do {
            tilesFile.open(tilesFileName);
            if (clock() - start > maxTime) {
                throw runtime_error(
                        "The file has not appeared in 10s. Game is going to end here. File location: " + tilesFileName);
            }

        } while (!tilesFile.is_open());

        // get the last line
        while (getline(tilesFile, line));

        tilesFile.close();

        // if line == Finished! compare will return false (0)
    }while (line.compare("Finished!"));

    scoresFile.open(scoresFileName);

    scoresFound = scoresFile.is_open();

    if (scoresFound) scoresFile.close();

    return scoresFound;
}

Mat getTilesFromFile(string file){
    ifstream myfile;
    string line;
    Mat tiles = Mat(Pool::ScreenHeight, Pool::ScreenWidth, CV_32F);

    myfile.open (file);
    int y = 0;
    // if line == Finished! compare will return false (0)
    while (getline(myfile, line) && line.compare("Finished!")){
        string number = "";

        int x = 0;
        for(char& c : line) {
            if (c == '[' || c == ' '){
                continue;
            }
            else if(c == ',' || c == ';' || c == ']'){
                tiles.at<float>(y, x) = stof(number);;
                number = "";
                x++;
            }
            else {
                number += c;
            }

        }

        y++;
    }

    myfile.close();


    //delete file
    std::remove(file.c_str());

    return tiles;
}

int getFitness(Mat screen, vector<Mat> sprites) {
    int height = screen.rows;
    int width = screen.cols;
    int tileHeight = 24;
    int tileWidth = 32;
    int fitness = 0;

    // split in 3 planes RGB
    vector<Mat> planes;
    split(screen, planes);

    int planeToCheck = 2;

    //variable that sets units, teens, hundreds, thousands.... for the fitness
    int mult = -1;


    // start from the right to the left reading the fitness in th screen
    for (int i = 15*tileHeight+1; i < (height / tileHeight) * tileHeight; i += tileHeight) {
        for (int j = (width / tileWidth) * tileWidth - tileWidth*8-6; j > 0 ; j -= tileWidth) {
            mult++;

            //get 8x8 block from image from (j,i) coordinates
            Mat block = planes[planeToCheck](cv::Rect(j, i, tileWidth, tileHeight));

            bool spriteFound = false;
            int mostSimilar = -1;
            int aeBnW = 500000000;

            for (int k = 0; k < sprites.size(); k++) {

                if (averageErrorBnW(sprites[k], block) < aeBnW) {
                    mostSimilar = k;
                    aeBnW = averageErrorBnW(sprites[k], block);

                    // if aeBnW is less than 200 it is assumed that it is the correct sprite
                    if (aeBnW < 200) {
                        spriteFound = true;
                    }
                }

                // if the spritez array has gonne through
                if (spriteFound || (k + 1) == sprites.size()) {

                    // if it does not detect any of the numbers, it found a blank tile, so it has finished reading the fitness
                    if (mostSimilar < 0 || !spriteFound && aeBnW > 1000) {
                        return fitness;
                    }

                    else {
                        fitness += mostSimilar*pow(10, mult);
                    }
                    break;
                }
            }
        }
    }

    return fitness;
}

void getFitnessSprites(vector<Mat>& sprites, char* location) {
    // do it for 10 digits
    for (int i = 0; i < 10; i++) {
        string n = to_string(i);

        char *image = new char[strlen(location)+6];
        strcpy(image, location);
        strcat(image, n.c_str());
        strcat(image, ".bmp");

        Mat sprite = imread(image, CV_LOAD_IMAGE_GRAYSCALE);
        if (!sprite.data) {
            cout << "Could not open or find the image in: " << image << endl;
        } else {
            cout << "Saving sprite: " << image << endl;
            sprites.push_back(sprite);
        }

        delete[] image;
    }
}

Mat scaleUp(Mat image, int scale){
    resize(image, image, image.size()*scale, 0, 0, INTER_AREA);   //resize image
    imshow("Tiles3", image);                   // Show our image inside it.
    moveWindow("Tiles3", 0, 500);
    return image;
}

string getDate(){
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );

    string sec = to_string(now->tm_sec);
    string min = to_string(now->tm_min);
    string hour = to_string(now->tm_hour);
    string time = hour + "-" + min + "-" + sec;

    string day = to_string(now->tm_mday);
    string month = to_string(now->tm_mon + 1);
    string year = to_string(now->tm_year + 1900);
    string date = day + ":" + month + ":" + year;

    return date+"_"+time;
}


//returns if player has died or not
bool playGameFromScreen(Pool pool, Mat *tiles, bool* running, vector<Mat> sprites, bool aiVision){
    // check keyPress
    int keyPressed = waitKeyEx(1);

    // if pressed ESC it closes the program
    if (keyPressed == 27) {
        *running = false;
    }

    Mat screen = getScreen();

    *tiles = getAveragesBnW(screen);

    // convert to floats
    tiles->convertTo(*tiles, CV_64FC1);


    //check if player has died.  The Blue values for the lives are between 40 and 70. if is less than that, there is no live in that tile
    // or if space is pressed. only for debugging
    if (tiles->at<double>(tiles->rows-2, 3) < 30 || keyPressed == 32){
        int fitness = getFitness(screen, sprites);

        pool.getGenomes()[pool.getCurrentGenome()]->setFitness(fitness);

        cout << "FITNESS: " << fitness << endl;

        return true;
    }

    // convert values between 0 and 1 being 1 255
    *tiles /= 255;

    if (aiVision) {
        Mat saveTiles = *tiles;
        scaleUp(saveTiles, 20);
    }
    return false;
}

bool playFromFile(Pool pool, Mat *tiles){
    string scoresFileName = "../../AI-SpaceInvaders/Score.txt";
    string tilesFileName = "../../AI-SpaceInvaders/tiles.txt";
    if (checkScoreFile(scoresFileName, tilesFileName)){
        ifstream myfile;
        myfile.open(scoresFileName);

        string line;

        getline(myfile, line);
        int fitness = stoi(line);

        myfile.close();

        //delete file
        std::remove(scoresFileName.c_str());

        pool.getGenomes()[pool.getCurrentGenome()]->setFitness(fitness);

        return true;
    }

    *tiles = getTilesFromFile(tilesFileName);

    return false;
}

void gnuplot(Pool pool) {
    // set terminal to x11 for mac
    Gnuplot::set_terminal_std("x11");

    // initialize the graph with the style in brackets
    Gnuplot g1("lines");
    g1.reset_plot();

    // set title of chart
    g1.set_title("Fitness Chart");
    g1.set_ylabel("Fitness");
    g1.set_xlabel("Generations");

    // do line with one
    g1.plot_x(pool.getMaxFitness(), "maxFitness");
    g1.plot_x(pool.getAverageFitness(), "averageFitness");
    //g1.plot_x(pool.getTotalFitness(), "totalFitness");

    // autoscale to the inputs given
    g1.set_xautoscale().replot();

    // window output
    g1.showonscreen();

    cout << endl << "Press ENTER to continue..." << endl;


    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    cin.get();

    g1.reset_all();
}

int main( int argc, char** argv ) {
    string saveLocation = "../Saves/"+getDate()+"0.6insteadOf0.4Disable.json";
    string loadLocation = "../Saves/24:5:2018_0-21-420.6insteadOf0.4Disable.json";
    bool poolFromFile = true;
    bool gameFromScreen = true;
    bool aiVision = false; // show what the AI sees
    bool* running = new bool(true);
    vector<Mat> sprites;       // collect the sprites for fitness


    Pool pool;
    if (poolFromFile){
        pool.loadPool(loadLocation);

        gnuplot(pool);

        cout << "Graph closed" << endl;
    }

    if (gameFromScreen){
        char* startLocation = "../Images/fitness/";
        getFitnessSprites(sprites, startLocation);
    }

    //infinite loop
    while(*running) {
        bool dead = false;

        if (gameFromScreen) {
            pressButton(7);
            sleep(1);
        }

        cout << "Current Generation: " << pool.getGeneration() << " Current Genome: " << pool.getCurrentGenome() << endl;
        cout << "Generating Network!" << endl;
        pool.getGenomes()[pool.getCurrentGenome()]->generateNetwork();
        pool.getGenomes()[pool.getCurrentGenome()]->showGenome();

        while (!dead) {
            Mat *tiles = new Mat;

            if (gameFromScreen){
                dead = playGameFromScreen(pool, tiles, running, sprites, aiVision);
            }
            else {
                dead = playFromFile(pool, tiles);
            }

            if (dead) continue;

            // START OF AI
            //create array of inputs
            vector<double> inputs;

            for (int i = 0; i < tiles->rows; i++){
                for (int j = 0; j < tiles->cols; j++){
                    double tile = tiles->at<float>(i, j);
                    inputs.push_back(tile);
                }
            }


            if (gameFromScreen){
                int buttonToPress = pool.getGenomes()[pool.getCurrentGenome()]->evaluateNetwork(inputs);
                pressButton(buttonToPress);
            }
            else {
                pool.getGenomes()[pool.getCurrentGenome()]->evaluateNetworkToFile(inputs);
            }
        }

        pool.nextGenome(saveLocation);

        // termination contition
        if (pool.getGeneration() >= 5000){
            *running = false;
        }
    }

    gnuplot(pool);
    cout << "DONE" << endl;
	putchar(getchar());
}