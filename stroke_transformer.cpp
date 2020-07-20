// Created by barth on 15.06.2020.

#include "stroke_transformer.h"

//vector<Point> CenterLine::transform(Mat processedImg) {

//}

vector< vector<Point> > Gestalt::transform(Mat processedImg){
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
            if (firstPixelBlack) { gridPoints.push_back(Point(x,y)); }
            if (secondPixelBlack) {gridPoints.push_back(Point (x+75, y+100));}
        }
    }
    vector< vector<Point> > vector_of_vecs;
    for (Point pt1 : gridPoints) {
        for (Point pt2 : gridPoints) {
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