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
//        Point a(4,4), b(5,4), c(3,4);
//        Vector2d d(a.x,a.y), e(b.x,b.y), f(c.x,c.y);
//        Vector2d x = (e - d).normalized();
//        Vector2d y = (f - e).normalized();
//
//        cout << x.dot(y);


        vector< vector<Point> > truncatedVectors;
        int c =0;
        for (vector<Point> contour : contours) {
            vector<Point> reducedPoints;
            reducedPoints.push_back(contour[0]);
            for (int i = 0; i < contour.size()-2; ++i) {
                Vector2d firstPoint(contour[i].x, contour[i].y), secondPoint(contour[i+1].x, contour[i+1].y), thirdPoint(contour[i+2].x, contour[i+2].y);
                Vector2d firstVector = (secondPoint - firstPoint).normalized();
                Vector2d secondVector = (thirdPoint - secondPoint).normalized();
                cout << firstVector.dot(secondVector) << " -- ";
//                if ((firstVector.dot(secondVector) >= 1) || (firstVector.dot(secondVector) <= -1)) {
//                    reducedPoints.push_back(contour[i + 1]);
//                    reducedPoints.push_back(contour[i + 2]);
//                }
//                if (-1 < firstVector.dot(secondVector) < 1) {
//                    reducedPoints.push_back(contour[i + 2]);
//                    if (reducedPoints[-1] == contour[i + 1]) { reducedPoints.push_back(contour[i + 2]); }
//                    else {
//                            reducedPoints.push_back(contour[i + 1]);
//                            reducedPoints.push_back(contour[i + 2]);
//                    }
//                }
            }
//            if(! reducedPoints.empty()) {
//                truncatedVectors.push_back(reducedPoints);
//                Mat img( 4000, 4000, CV_8UC3, Scalar(255,255,255));
//                for (int i = 0; i < reducedPoints.size()-1; i++ ) {
//                    circle(img, reducedPoints[i], 1, Scalar( 0, 0, 0 ), FILLED, LINE_8);
//                }
//                c++;
//                imwrite("cmake-build-debug/debug-pics/pic" + to_string(count) + "--" + to_string(c) + ".jpg", img);
//            }
            cout << endl;
            cout << endl;
        }

        std::ofstream ped;
        ped.open("cmake-build-debug/ped_files/pts-" + to_string(count) + ".ped", ios::out | ios::trunc );
        ped << "Stroke CoordinateSystem Origin (0, 0, 0) Max (5000, 5000, 1)" << endl;
        ped << endl;
//        sort(contours.begin(), contours.end(), [](vector<Point> & a, vector<Point> & b) {return a.size() > b.size();} );
        for (vector<Point> vec : truncatedVectors) {
            string pt;
            for (Point pts : vec) {
                pt += "(" + to_string(pts.x) + ", " + to_string(pts.y) + ") ";
            }
            ped << pt << endl;
        }
        ped.close();



        for (int i = 0; i < truncatedVectors.size(); i++ ) {
            drawContours( contoured, truncatedVectors, (int)i, Scalar(0,0,0), 2, LINE_8, hierarchy, 0 );
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