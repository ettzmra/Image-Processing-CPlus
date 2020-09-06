// Created by barth on 15.06.2020.

#include "stroke_transformer.h"

vector< vector<Point> > CenterLine::transform(Mat src) {
    Process img(src);
    Mat binarized = img.binarize(THRESH_BINARY_INV, 101);
    Mat processedImg;
    Mat dilate_kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(2, 2));
    morphologyEx(binarized, processedImg, MORPH_CLOSE, dilate_kernel, Point(-1,-1), 21);
    morphologyEx(processedImg, processedImg, MORPH_OPEN, dilate_kernel, Point(-1,-1), 21);
    ximgproc::thinning(processedImg, processedImg);
//    threshold(processedImg, processedImg, 170, 255, THRESH_BINARY);
//    namedWindow("bin", WINDOW_NORMAL);
//    imshow("bin", processedImg);
//    waitKey();

    vector< vector<Point> > vecs;
    vector<Point> endPts, crossPts, midPts;

    int channels = processedImg.channels();
    cout << "yes" << to_string(channels);

    int rows = processedImg.rows;
    int cols = processedImg.cols * channels;
    int x = 1;
    int y = 1;
   // uchar& pxl = processedImg.at<uchar>(x, y);
   // cout << pxl;

//    while((y < cols-1) && (x < rows-1)) {
//        Vec3b& pxl = processedImg.at<uchar>(x, y);
//        int count = 0;
//        vector<Point> neighborPxls;
//        if(pxl == Vec3b(255,255,255)) {
//            for (int i = -1; i <= 1; i++) {
//                for (int e = -1; e <= 1; e++) {
//                    Vec3b &pxlColor = processedImg.at<Vec3b>(x + e, y + i);
//                    if ((pxlColor == Vec3b(255, 255, 255)) && !(i == 0 && e == 0)) {
//                        neighborPxls.push_back(Point(x + e, y + i));
//                        count++;
//                    }
//                }
//            }
//            //Vec3b &color = processedImg.at<Vec3b>(x, y);
//            if (count == 1) {
//                Vec3b color = Vec3b(0, 255, 0);
//                processedImg.at<Vec3b>(Point(x,y)) = color;
//            } else if (count == 2) {
//                Vec3b color = Vec3b(255, 255, 0);
//                processedImg.at<Vec3b>(Point(x,y)) = color;
//            } else if (count > 3) {
//                Vec3b color = Vec3b(255, 0, 0);
//                processedImg.at<Vec3b>(Point(x,y)) = color;
//                crossPts.push_back(Point(x, y));
//            }
//            if (neighborPxls.empty()) {
//                x++;
//                y++;
//            } else {
//                x = neighborPxls[0].x;
//                y = neighborPxls[0].y;
//            }
//        } else {x++; y++;}
//    }
//    namedWindow("bin", WINDOW_NORMAL);
//    imshow("bin", processedImg);
//    waitKey();

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