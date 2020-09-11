// Created by barth on 21.06.2020.

#include "imgProcessing.h"

Process::Process(Mat src) : img(src) {}

Mat Process::binarize(ThresholdTypes flag, int blur){
    Mat src_gray, blurred, binarized;
    medianBlur(img, blurred, blur);
    cvtColor(img, src_gray, COLOR_BGR2GRAY);
    threshold(src_gray, binarized, 170, 255, flag);
    return binarized;
}

Mat Process::fillHoles(Mat binarized) {
    Mat dilated, eroded;
    Mat erode_kernel = getStructuringElement(MORPH_RECT, Size(9, 9), Point(2, 2));
    Mat dilate_kernel = getStructuringElement(MORPH_RECT, Size(21, 21), Point(2, 2));
    dilate(binarized, dilated, dilate_kernel);
    erode(dilated, eroded, erode_kernel);
    return eroded;
}

vector< vector<Point> > Process::contours(Mat processedImg, vector<Vec4i>& hierarchy) {
    Canny(processedImg, canny, 170, 255);
    vector< vector<Point> > contours;
    findContours(canny, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    return contours;
}

vector<vector<Point>> Process::truncateVectors(vector< vector<Point> >& contours){
    vector< vector<Point> > truncVectors;
//    int c = 0;
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
    return truncVectors;
}


void draw_outputs(int output_num, vector<vector<Point>>& truncVectors, vector<Vec4i>& hierarchy, Mat canny) {
    Mat output(canny.size(), CV_8UC3, Scalar(255,255,255));
    for (int i = 0; i < truncVectors.size(); i++ ) {
        drawContours(output, truncVectors, (int)i, Scalar(0,0,0), 2, LINE_8, hierarchy, 0);
    }
    namedWindow("dil", WINDOW_NORMAL);
    imshow("dil", output);
    waitKey();
    imwrite("output images/skel" + to_string(output_num) + ".jpg", output);
    Mat img(canny.size(), CV_8UC3, Scalar(255,255,255));
//    for(vector<Point> vec : truncVectors) {
//        if(truncVectors[1] == vec) {
//            for (int i = 0; i < vec.size() - 1; i++) {
//                cv::line(img, vec[i], vec[i + 1], Scalar(0, 0, 0), 2, LINE_8);
//                cv::circle(img, vec[i], 4, Scalar(0, 255, 0), FILLED, LINE_8);
//            }
//            cout << vec.size() << " -- ";
//            imwrite("output images/debugPic" + to_string(output_num) + ".jpg", img);
//        }
//    }
}


void points_to_ped(string name, int img_num, vector < vector<Point> >& vectors){
    std::ofstream ped;
    ped.open("ped_files/" + name + "_" + to_string(img_num) + ".ped", ios::out | ios::trunc );
    ped << "Stroke CoordinateSystem Origin (0, 0, 0) Max (5000, 5000, 1)" << endl;
    ped << endl;
    sort(vectors.begin(), vectors.end(), [](vector<Point> & a, vector<Point> & b) {return a.size() > b.size();} );
    for (vector<Point>& vec : vectors) {
        string pt;
        for (Point& pts : vec) {
            pt += "(" + to_string(pts.x) + ", " + to_string(pts.y) + ") ";
        }
        ped << pt << endl;
    }
    ped.close();
}

vector<Point> skeletonEndPoints(Mat& src) {
    vector<Point> endPts;

    for(int y = 1; y < src.cols-2; y++) {
        for (int x = 1; x < src.rows-2; x++) {
            Vec3b pxl = src.at<Vec3b>(x,y);
            int count = 0;
            vector<Point> neighborPxls;
            bool hitCanvas = false;
            if (pxl == Vec3b(255,255,255)) {
                for (int i = -1; i <= 1; i++) {
                    for (int e = -1; e <= 1; e++) {
                        Vec3b neighborPxl = src.at<Vec3b>(x+e,y+i);
                        Point neighborPt = Point(x+e, y+i);
                        if (neighborPxl == Vec3b(255,255,255) && !(i == 0 && e == 0)) {
                            neighborPxls.push_back(Point(x + e, y + i));
                            if((neighborPt.x == (src.rows-1)) || (neighborPt.y == (src.cols-1))) {
                                hitCanvas = true;
                            }
                            count++;
                        }
                    }
                }
            }
            if (((count == 1) && (x != (src.rows-1)) && (y != (src.cols-1))) || hitCanvas) {
                src.at<Vec3b>(x, y) = Vec3b(0,255,0);
                endPts.push_back(Point(x,y));
            }
        }
    }
    return endPts;
}

Point neighborPixel(Mat& src, Point p) {
    vector<Point> neighbors;
    for (int i = -1; i <= 1; i++) {
        for (int e = -1; e <= 1; e++) {
            Vec3b pxl = src.at<Vec3b>(p.x+e,p.y+i);
            Point pxlPt = Point(p.x + e, p.y + i);
            if (pxl == Vec3b(255,255,255) && !(i == 0 && e == 0)) {
                neighbors.push_back(pxlPt);
            }
        }
    }
    if(neighbors.empty()) return p;
    vector<int> count;
    for(Point pt : neighbors) {
        int c;
        for (int i = -1; i <= 1; i++) {
            for (int e = -1; e <= 1; e++) {
                Vec3b pxl = src.at<Vec3b>(pt.x+e,pt.y+i);
                if (pxl == Vec3b(255,255,255) && !(i == 0 && e == 0)) c++;
            }
        }
        count.push_back(c);
    }
    int max = 0;
    int idx = 0;
    for(int i = 0; i < count.size(); i++) {
        if(count[i] > max) {
            max = count[i];
            idx = i;
        }
    }
    return neighbors[idx];
}

void walkNeighbours(Mat& src, vector<Point>& endPts, vector< vector<Point> >& v) {
    for(Point pt : endPts) {
        if(src.at<Vec3b>(pt) == Vec3b(0, 0, 255)) continue;
        else {
            vector<Point> points;
            src.at<Vec3b>(pt.x,pt.y) = Vec3b(0, 0, 255);
            points.push_back(pt);
            Point neighbour = neighborPixel(src, pt);
            while (find(endPts.begin(), endPts.end(), neighbour) == endPts.end()) {
                src.at<Vec3b>(neighbour.x, neighbour.y) = Vec3b(0, 0, 255);
                points.push_back(neighbour);
                pt = neighbour;
                neighbour = neighborPixel(src, pt);
                if(neighbour == pt) break;
            }
            v.push_back(points);
        }
    }
}

vector<Point> skeletonCrossPoints(Mat& src) {
    vector<Point> crossPts;

    int rows = src.rows;
    int cols = src.cols;

    for(int y = 1; y < cols-1; y++) {
        for (int x = 1; x < rows - 1; x++) {
            Vec3b pxl = src.at<Vec3b>(x,y);
            vector<Point> neighborPxls;
            if (pxl == Vec3b(255,255,255)) {
                for (int i = -1; i <= 1; i++) {
                    for (int e = -1; e <= 1; e++) {
                        Vec3b neighborPxl = src.at<Vec3b>(x+e,y+i);
                        if (neighborPxl != Vec3b(0,0,0) && !(i == 0 && e == 0)) {
                            neighborPxls.push_back(Point(x + e, y + i));
                        }
                    }
                }
            }
            if (neighborPxls.size() > 2) {
                for(auto each : neighborPxls) {
                    int count = 0;
                    for (int i = -1; i <= 1; i++) {
                        for (int e = -1; e <= 1; e++) {
                            Vec3b neighbor = src.at<Vec3b>(each.x + e, each.y + i);
                            if (neighbor != Vec3b(0, 0, 0) && !(i == 0 && e == 0)) {
                                count++;
                            }
                        }
                    }
                    if (count == 2) {
                        if(find(crossPts.begin(), crossPts.end(), Point(x,y)) == crossPts.end()) {
                            src.at<Vec3b>(x, y) = Vec3b(0, 0, 255);
                            crossPts.push_back(Point(x, y));
                        }
                    }
                }
            }
        }
    }
    return crossPts;
}
