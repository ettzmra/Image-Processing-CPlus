// Created by barth on 15.06.2020.

#include "stroke_transformer.h"
#include <cstdlib>


vector< vector<Point> > CenterLine::transform(Mat src) {
    assert(src.channels() == 3);
    Process img(src);
    Mat binarized = img.binarize(THRESH_BINARY_INV, 101);
    Mat processedImg;
    Mat dilate_kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(2, 2));
    morphologyEx(binarized, processedImg, MORPH_CLOSE, dilate_kernel, Point(-1,-1), 21);
    morphologyEx(processedImg, processedImg, MORPH_OPEN, dilate_kernel, Point(-1,-1), 21);
    ximgproc::thinning(processedImg, processedImg);
    cv::cvtColor(processedImg, processedImg, cv::COLOR_GRAY2BGR);

    int rows = src.rows;
    int cols = src.cols;
    vector< vector<Point> > vecs;
    vector<Point> endPoints1 = skeletonEndPoints(processedImg);
    walkNeighbours(processedImg, endPoints1, vecs);

//    int count = 0;
//    for (int y = 1; y < cols - 1; y++) {
//        for (int x = 1; x < rows - 1; x++) {
//            Vec3b pxl = src.at<Vec3b>(x, y);
//            if (pxl == Vec3b(255, 255, 255)) {
//                count++;
//            }
//        }
//    }
//    if (count > 50) {
//        vector<Point> endPoints2 = skeletonEndPoints(processedImg);
//        walkNeighbours(processedImg, endPoints2, vecs);
//    }

    //namedWindow("bin", WINDOW_NORMAL);
   // if(boost::filesystem::exists("bin.png"))
    int c = rand();
    imwrite(to_string(c) + "bin.png", processedImg);

    return vecs;
}

vector< vector<Point> > Gestalt::transform(Mat src){
    Process img(src);
    Mat binarized = img.binarize();
    Mat processedImg = img.fillHoles(binarized);
    vector<Point> gridPoints;
    Mat color(1,1,CV_8UC1,Scalar(0,0,0));  // black
//    Mat img( 4000, 4000, CV_8UC3, Scalar(255,255,255));  // the canvas our gestalt transformation is drawn.
    int channels = processedImg.channels();
    int rows = processedImg.rows;
    int cols = processedImg.cols * channels;
    for (int y = 0; y < rows-100; y += 200){
        for (int x = 0; x < cols-75; x += 150){
            Mat firstRowPixel = processedImg.row(y).col(x);  // pixel value. Either [0](black) or [255](white)
            Mat secondRowPixel = processedImg.row(y+100).col(x+75);
            bool firstPixelBlack = countNonZero(firstRowPixel != color) == 0;   // compares and determines whether the colour of the pixel is black.
            bool secondPixelBlack = countNonZero(secondRowPixel != color) == 0;
            if (firstPixelBlack) gridPoints.push_back(Point(x,y));
            if (secondPixelBlack) {gridPoints.push_back(Point (x+75, y+100));}
        }
    }
    vector< vector<Point> > vector_of_vecs;
    for (const Point& pt1 : gridPoints) {
        for (const Point& pt2 : gridPoints) {
            if((abs(pt1.y - pt2.y) <= 100) && (abs(pt1.x - pt2.x) <= 150)) {  // gives squares when abs(pt1.x - pt2.x) <= 80
                vector<Point> vec;
                vec.push_back(pt1);
                vec.push_back(pt2);
                vector_of_vecs.push_back(vec);
            }
        }
    }
    return vector_of_vecs;
}
