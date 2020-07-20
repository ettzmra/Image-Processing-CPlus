// Created by barth on 15.06.2020.

#ifndef STROKEPROCESSING_STROKE_TRANSFORMER_H
#define STROKEPROCESSING_STROKE_TRANSFORMER_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <Eigen/Geometry>


using namespace Eigen;
using namespace cv;
using namespace std;
using namespace boost::filesystem;

class Transformer {
public:
    virtual vector< vector<Point> > transform (Mat processedImg) = 0;
};

class CenterLine : public Transformer {
public:
    vector< vector<Point> > transform (Mat processedImg) override;
};

class Gradience : public Transformer {
public:
    vector< vector<Point> > transform (Mat processedImg) override;
};

class Gestalt : public Transformer {
public:
    vector< vector<Point> > transform (Mat processedImg) override;

};

#endif //STROKEPROCESSING_STROKE_TRANSFORMER_H
