// Created by barth on 22.06.2020.


#ifndef STROKEPROCESSING_IMGPROCESSING_H
#define STROKEPROCESSING_IMGPROCESSING_H

#include "stroke_transformer.h"

using namespace cv;
using namespace std;

class Process : Mat {
public:
    Mat img, canny;
    Process(Mat src);
    Mat binarize(ThresholdTypes flag = THRESH_BINARY, int blur=101);
    Mat fillHoles(Mat binarized);
    vector< vector<Point> > contours(Mat processedImg, vector<Vec4i>& hierarchy);
    vector<vector<Point>> truncateVectors(vector<vector<Point> > &contours);
};

void draw_outputs(int output_num, vector<vector<Point>> &truncVectors, vector<Vec4i>& hierarchy, Mat canny);

void points_to_ped(string name, int img_num, vector<vector<Point> > vectors);

vector<Point> skeletonEndPoints(Mat& src);
vector<Point> skeletonCrossPoints(Mat& src);
Point neighborPixel(Mat& src, Point p);
void walkNeighbours(Mat& src, vector<Point>& endPts, vector< vector<Point> >& v);

#endif //STROKEPROCESSING_IMGPROCESSING_H
