//
//  main.cpp
//  SDC Animation
//
//  Created by Joon Kang on 2019-12-29.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;
using namespace cv;

//==============================================================================
// Source: https://stackoverflow.com/questions/40895785/using-opencv-to-overlay-
//         transparent-image-onto-another-image
//==============================================================================
// Function used to apply alpha mask with a given min and max threshold
//==============================================================================
Mat applyAlphaMask(int minThresh, int maxThresh, Mat background, Mat forground, int row, int col) {
    Mat dst;
    Mat tmp,alpha;
    
    cvtColor(forground,tmp,COLOR_BGR2GRAY);
    threshold(tmp,alpha,minThresh,maxThresh,THRESH_BINARY);
    
    Mat rgb[3];
    split(forground,rgb);
    
    Mat rgba[4]={rgb[0],rgb[1],rgb[2],alpha};
    merge(rgba,4,forground);
    
    Mat mask;
    vector<Mat> layers;
    
    split(forground, layers); // seperate channels
    Mat rgb2[3] = { layers[0],layers[1],layers[2] };
    mask = layers[3]; // png's alpha channel used as mask
    merge(rgb2, 3, forground);  // put together the RGB channels, now transp insn't transparent
   
    Mat outputImg;
    background.copyTo(outputImg);
    
    forground.copyTo(outputImg.rowRange(row, row + forground.rows).colRange(col, col + forground.cols), mask);
   
    return outputImg;
}

int main(int argc, const char * argv[]) {

    // Obtain file path to the assets
    std::string temp(argv[0]);
    string filePath = temp.substr(0,temp.length()-13);
    
    //--------------------------------------------------------------------------
    // Create VideoCapture objects for our video files
    //--------------------------------------------------------------------------
    
    // Pre-recorded video of a counter counting up from 0 to 300,000 where every
    // frame the counter increases by 100
    VideoCapture cap(filePath + "counter.mov");
    
    // Pre-recorded video of snow-like particle effects
    VideoCapture cap2(filePath + "transparent_1.mp4");
    
//    VideoCapture cap3(filePath + "transparent_2.mp4")
//    VideoCapture cap4(filePath + "fireworks.mp4");
    
    // Pre-recorded video of a looping water wave
    VideoCapture cap5(filePath + "waterlevel.mov");
    
    
    //--------------------------------------------------------------------------
    // Intialize variables
    //--------------------------------------------------------------------------

    // Bottom-most layer that acts like the background image
    Mat blue = imread(filePath + "blue.png");
    Size size(960,1920);
    resize(blue, blue, size);
    
    // the progress bar that increases as the donation counter goes up
    Mat lightblue = imread(filePath + "light_blue.png");
   
    
    // Used to keep track of the current donation amount
    int count = 1;
    int framecount = 0;
    
    // Used to store the different layers
    Mat background;
    Mat prevBackground;

    Mat waterlevel;
    Mat prevWaterLevel;
    
    //--------------------------------------------------------------------------
    // Main loop
    //--------------------------------------------------------------------------
    while(true) {
        // Checks to see if the counter needs to be increased, and if so, reads in a single frame of the pre-recorded counter video
        if (framecount < count) {
            
            background.copyTo(prevBackground);
            cap >> background;

            if (background.empty()) {
                prevBackground.copyTo(background);
            }
            Size size(960,1920);
            resize(background,background,size); //resize image
            
            framecount++;
        }
        
        // Uses the current frame count to calculate and resize the light blue progress bar accordingly
        Mat curImg;
        if ((1920 - (framecount*0.801+1) - 35) >= 0) {
            Size sizeLightBlue(960,framecount*0.801+1);
            resize(lightblue, lightblue, sizeLightBlue);
            curImg = applyAlphaMask(10, 255, blue, lightblue, 1920 - (framecount*0.801+1), 0);
        }
        else {
            Size sizeLightBlue(960,1920 - 35);
            resize(lightblue, lightblue, sizeLightBlue);
            curImg = applyAlphaMask(10, 255, blue, lightblue, 35, 0);
        }
        
        
        // The water wave loop is calculated and placed seemlessly on top of the progress bar to add animation
        waterlevel.copyTo(prevWaterLevel);
        cap5 >> waterlevel;
        
        if (waterlevel.empty()) {
            cap5 = VideoCapture(filePath + "waterlevel.mov");
            prevWaterLevel.copyTo(waterlevel);
        }
        if ((1920 - (framecount*0.801+1) - 35) >= 0) {
            curImg = applyAlphaMask(100, 255, curImg, waterlevel, 1920 - (framecount*0.801+1) - 35, 0);
        }
        else {
            curImg = applyAlphaMask(100, 255, curImg, waterlevel, 0, 0);
        }
        
        // Combine background layer with progress bar layer
        Mat temp;
        background.copyTo(temp);
        temp = applyAlphaMask(10, 255, curImg, temp, 0, 0);

        // Apply the top most layer (snow-like particles)
        Mat forground;
        cap2 >> forground;

        if (forground.empty()) {
            // If the video is finished, restart the loop
            cap2 = VideoCapture(filePath + "transparent_1.mp4");
        }
        else {
            Size size(960,1920);
            resize(forground,forground,size); //resize image
            
            // Combine all previous layers with snow-like particles layer
            Mat outputImg;
            outputImg = applyAlphaMask(250, 255, temp , forground, 0, 0);

            // Display the resulting frame
            imshow("frame", outputImg);
        }
        
        // If spacebar is pressed, play the counter video for 5 frames
        char c=(char)waitKey(25);
        if(c==32)
            count += 5;
    }
    
//    cout << "count: " << count << endl;
//    cout << "framecount: " << count << endl;


    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();
    return 0;
}
