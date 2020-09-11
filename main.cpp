
#include "imgProcessing.h"
#include "stroke_transformer.h"

int main()
{
    int count = 1;
    for(auto& file : directory_iterator("smallPics")) {  // loop through the directory where the original stroke images are located.
        Mat src = imread(file.path().string(), 1); // Read the file
        CenterLine c;
        vector< vector<Point> > pts = c.transform(src);
        Mat output(src.size(), CV_8UC3, Scalar(255,255,255));
        for(vector<Point> pt : pts) {
            for (int i = 0; i < pt.size(); i++) {
                if((i == 0) || (i == (pt.size()-1))) {
                    cv::circle(output, pt[i], 4, Scalar(0, 0, 255), FILLED, LINE_8);
                } else { cv::line(output, pt[i], pt[i + 1], Scalar(0, 255, 0), 2, LINE_8); }
            }
        }
        namedWindow("bin", WINDOW_NORMAL);
        imshow("bin", output);
        waitKey();
        imwrite("output images/center_line" + to_string(count) + ".jpg", output);
//        Mat skel = Mat (binarized.rows, binarized.cols, CV_8UC1, Scalar (0,0,0));
        //       Gestalt g;
 //       vector< vector<Point> > pts = g.transform(eroded);
//        Mat contoured(canny.size(), CV_8UC3, Scalar(255,255,255));
//        draw_outputs(count, contours, hierarchy, contoured);
//        findContours( skel, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
//        vector< vector<Point> > refined_vectors = truncateVectors(contours);
//        draw_outputs(count, contours, hierarchy, contoured);

        count++;
//        points_to_ped("triangles", count, pts);



    }
    return 0;
}








//
//for (int i = 0; i < truncVectors.size(); i++ ) {
//drawContours( contoured, truncVectors, (int)i, Scalar(0,0,0), 2, LINE_8, hierarchy, 0 );
//}
//
//imwrite("cmake-build-debug/output images/filtered" + to_string(count) + ".jpg", contoured);


//        medianBlur(src, blurred, 101);
//        cvtColor(blurred, src_gray, COLOR_BGR2GRAY);
//        threshold(src_gray, binarized, 170, 255, THRESH_BINARY);



//        vector< vector<Point> > truncVectors;
//        int c =0;
//        for (vector<Point> contour : contours) {
//            vector<Point> reducedPoints;
//            vector<Point> truncContour;
//            for (int i = 0; i < contour.size(); i += 2) { reducedPoints.push_back(contour[i]); }
//            truncContour.push_back(reducedPoints[0]);
//            for (int i = 0; i < reducedPoints.size()-2; i += 2) {
//                Vector2d firstPoint(reducedPoints[i].x, reducedPoints[i].y), secondPoint(reducedPoints[i+1].x, reducedPoints[i + 1].y), thirdPoint(reducedPoints[i + 2].x, reducedPoints[i + 2].y);
//                Vector2d firstVector = (secondPoint - firstPoint).normalized();
//                Vector2d secondVector = (thirdPoint - secondPoint).normalized();
//                if ((firstVector.dot(secondVector) >= 0.99) || (firstVector.dot(secondVector) <= -0.99)) {}
//                else { truncContour.push_back(reducedPoints[i + 2]); }
//            }
//            if (! truncContour.empty()) {
//                truncVectors.push_back(truncContour);
////                Mat img( 4000, 4000, CV_8UC3, Scalar(255,255,255));
////                for (int i = 0; i < truncContour.size()-1; i++ ) {
////                    if (c == 10) {
////                        line(img, truncContour[i], truncContour[i+1], Scalar( 0, 0, 0 ), 2, LINE_8);
////                        circle(img, truncContour[i], 2, Scalar( 0, 255, 0 ), FILLED, LINE_8);
////                    }
////                    else { circle(img, truncContour[i], 1, Scalar( 0, 0, 0 ), FILLED, LINE_8); }
////                }
////                c++;
////                cout << truncContour.size() << " -- ";
////                imwrite("cmake-build-debug/debug-pics/pic" + to_string(count) + "--" + to_string(c) + ".jpg", img);
//            }
//        }




//        std::ofstream ped;
//        ped.open("cmake-build-debug/ped_files/pts-" + to_string(count) + ".ped", ios::out | ios::trunc );
//        ped << "Stroke CoordinateSystem Origin (0, 0, 0) Max (5000, 5000, 1)" << endl;
//        ped << endl;
//        sort(truncVectors.begin(), truncVectors.end(), [](vector<Point> & a, vector<Point> & b) {return a.size() > b.size();} );
//        for (vector<Point> vec : truncVectors) {
//            string pt;
//            for (Point pts : vec) {
//                pt += "(" + to_string(pts.x) + ", " + to_string(pts.y) + ") ";
//            }
//            ped << pt << endl;
//        }
//        ped.close();


//        for contour in vec<vec<point>>
//        img = new image
//        for p in contour
//        cv::point(p , img)
//        saveImg(img)
//        cout << file.path().string() << newvec.size() << "---" << newvec[0] << endl;
//        cout << contours.size() << "----" << contours.size() << endl;