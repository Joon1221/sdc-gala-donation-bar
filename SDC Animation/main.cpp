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

Mat applyAlphaMask(int minThresh, int maxThresh, Mat background, Mat forground, int row, int col) {
    
    Mat dst;//(src.rows,src.cols,CV_8UC4);
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
    // insert code here...
    cout << "Hello1" << endl;
//    cout << argv[0] << endl;
    cout << "Hello2" << endl;

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    std::string temp(argv[0]);
    string filePath = temp.substr(0,temp.length()-13);
    
    VideoCapture cap(filePath + "counter.mov");
    VideoCapture cap2(filePath + "transparent_1.mp4");
//    VideoCapture cap3("transparent_1.mp4");
    VideoCapture cap4(filePath + "fireworks.mp4");
    VideoCapture cap5(filePath + "waterlevel.mov");
    
    Size size(960,1920);//the dst image size,e.g.100x100
    Mat blue = imread(filePath + "blue.png");
    Mat lightblue = imread(filePath + "light_blue.png");
    
    resize(blue, blue, size);
    
    // Check if camera opened successfully
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    int count = 1;
    int framecount = 0;
    bool goalReached = false;
    
    Mat background;
    Mat prevBackground;

    Mat waterlevel;
    Mat prevWaterLevel;
    
    while(true) {
        Mat curImg;
        if (framecount < count) {
//            if (framecount == 10000) {
//                goalReached = true;
//            }
            
            //delay
//            usleep(50000);
            background.copyTo(prevBackground);
            cap >> background;


            // If the frame is empty, break immediately
            if (background.empty()) {
                prevBackground.copyTo(background);
            }
            Size size(960,1920);//the dst image size,e.g.100x100
            resize(background,background,size);//resize image
            
            framecount++;
        }
        

        
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
        Mat temp;
        background.copyTo(temp);
        temp = applyAlphaMask(10, 255, curImg, temp, 0, 0);

        Mat forground;
        cap2 >> forground;

        if (forground.empty()) {
            cap2 = VideoCapture(filePath + "transparent_1.mp4");
        }
        else {
            Size size(960,1920);//the dst image size,e.g.100x100
            resize(forground,forground,size);//resize image
            
            Mat outputImg;
            outputImg = applyAlphaMask(250, 255, temp , forground, 0, 0);


            if (goalReached) {
//            if (false) {
                Mat firework;
                cap4 >> firework;
                Size size(960,1920);//the dst image size,e.g.100x100
                if (firework.empty()) {
                    cout << "hello3" << endl;

                    goalReached = false;
                    
                }
                else {
                    resize(firework,firework,size);//resize image

//                    cout << "hello1" << endl;
                    Mat dst1;//(src.rows,src.cols,CV_8UC4);
                    Mat tmp1,alpha1;
                    
                    cvtColor(firework,tmp1,COLOR_BGR2GRAY);
                    threshold(tmp1,alpha1,100,255,THRESH_BINARY);
                    
                    Mat rgb1[3];
                    split(firework,rgb1);
                    
                    Mat rgba1[4]={rgb1[0],rgb1[1],rgb1[2],alpha1};
                    merge(rgba1,4,firework);
                    
                    //============================================================
                    // APPLY MASK
                    //============================================================
                    
                    Mat mask1;
                    vector<Mat> layers1;
                    
                    split(firework, layers1); // seperate channels
                    Mat rgb21[3] = { layers1[0],layers1[1],layers1[2] };
                    mask1 = layers1[3]; // png's alpha channel used as mask
                    merge(rgb21, 3, firework);  // put together the RGB channels, now transp insn't transparent
//                    Mat outputImg1;
//                    outputImg.copyTo(outputImg1);
                    
                    firework.copyTo(outputImg.rowRange(0, 0 + firework.rows).colRange(0, 0 + firework.cols), mask1);
//                    cout << "hello2" << endl;

                }
            }
            Size sizeOutput(1440, 900);
            resize(outputImg,outputImg,sizeOutput);//resize image

            // Display the resulting frame
            imshow("frame", outputImg);
        }
        // Press  ESC on keyboard to exit
        char c=(char)waitKey(25);
        if(c==32)
            count += 5;
    }
    waitKey();
    
    cout << "count: " << count << endl;
    cout << "framecount: " << count << endl;


    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();
    return 0;
}
