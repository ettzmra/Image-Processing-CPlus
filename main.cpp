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

int main()
{
    int count = 0;
    for(auto& file : directory_iterator("cmake-build-debug/strokes_images")) {
        Mat src, src_gray, canny, blurred, binarized, dilated, eroded;
        src = imread(file.path().string(), 1); // Read the file
        medianBlur(src, blurred, 101);
        cvtColor(blurred, src_gray, COLOR_BGR2GRAY);
        threshold(src_gray, binarized, 170, 255, THRESH_BINARY);
        Mat erode_kernel = getStructuringElement( MORPH_RECT, Size(9,9), Point(2, 2) );
        Mat dilate_kernel = getStructuringElement( MORPH_RECT, Size(21,21), Point(2, 2) );
        dilate(binarized, dilated, dilate_kernel);
        erode(dilated, eroded, erode_kernel);
        Canny( eroded, canny, 170, 255 );
        vector< vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours( canny, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
        Mat contoured(canny.size(), CV_8UC3, Scalar(255,255,255));
        count++;

        vector< vector<Point> > truncVectors;
        int c =0;
        for (vector<Point> contour : contours) {
            vector<Point> reducedPoints;
            vector<Point> truncContour;
            for (int i = 0; i < contour.size(); i += 2) { reducedPoints.push_back(contour[i]); }
            truncContour.push_back(reducedPoints[0]);
            for (int i = 0; i < reducedPoints.size()-2; i += 2) {
                Vector2d firstPoint(reducedPoints[i].x, reducedPoints[i].y), secondPoint(reducedPoints[i+1].x, reducedPoints[i + 1].y), thirdPoint(reducedPoints[i + 2].x, reducedPoints[i + 2].y);
                Vector2d firstVector = (secondPoint - firstPoint).normalized();
                Vector2d secondVector = (thirdPoint - secondPoint).normalized();
                if ((firstVector.dot(secondVector) >= 0.99) || (firstVector.dot(secondVector) <= -0.99)) {}
                else { truncContour.push_back(reducedPoints[i + 2]); }
            }
            if (! truncContour.empty()) {
                truncVectors.push_back(truncContour);
//                Mat img( 4000, 4000, CV_8UC3, Scalar(255,255,255));
//                for (int i = 0; i < truncContour.size()-1; i++ ) {
//                    if (c == 10) {
//                        line(img, truncContour[i], truncContour[i+1], Scalar( 0, 0, 0 ), 2, LINE_8);
//                        circle(img, truncContour[i], 2, Scalar( 0, 255, 0 ), FILLED, LINE_8);
//                    }
//                    else { circle(img, truncContour[i], 1, Scalar( 0, 0, 0 ), FILLED, LINE_8); }
//                }
//                c++;
//                cout << truncContour.size() << " -- ";
//                imwrite("cmake-build-debug/debug-pics/pic" + to_string(count) + "--" + to_string(c) + ".jpg", img);
            }
        }
//        cout << endl;
//        cout << endl;

        std::ofstream ped;
        ped.open("cmake-build-debug/ped_files/pts-" + to_string(count) + ".ped", ios::out | ios::trunc );
        ped << "Stroke CoordinateSystem Origin (0, 0, 0) Max (5000, 5000, 1)" << endl;
        ped << endl;
        sort(truncVectors.begin(), truncVectors.end(), [](vector<Point> & a, vector<Point> & b) {return a.size() > b.size();} );
        for (vector<Point> vec : truncVectors) {
            string pt;
            for (Point pts : vec) {
                pt += "(" + to_string(pts.x) + ", " + to_string(pts.y) + ") ";
            }
            ped << pt << endl;
        }
        ped.close();

        for (int i = 0; i < truncVectors.size(); i++ ) {
            drawContours( contoured, truncVectors, (int)i, Scalar(0,0,0), 2, LINE_8, hierarchy, 0 );
        }
//        for contour in vec<vec<point>>
//        img = new image
//        for p in contour
//        cv::point(p , img)
//        saveImg(img)
//        cout << file.path().string() << newvec.size() << "---" << newvec[0] << endl;
//        cout << contours.size() << "----" << contours.size() << endl;
        imwrite("cmake-build-debug/output images/filtered" + to_string(count) + ".jpg", contoured);
    }
    return 0;
}