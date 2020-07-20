// Created by barth on 22.06.2020.


#ifndef STROKEPROCESSING_IMGPROCESSING_H
#define STROKEPROCESSING_IMGPROCESSING_H

#include "stroke_transformer.h"

Mat binarize(Mat src);

vector<vector<Point>> truncateVectors(vector< vector<Point> > contours);

void draw_outputs(int output_num, vector<vector<Point>> truncVectors, vector<Vec4i> hierarchy, Mat output);

void points_to_ped(string name, int img_num, vector < vector<Point> > vectors);



#endif //STROKEPROCESSING_IMGPROCESSING_H
