//
// Created by Damien Dabernat on 02/11/2022.
//

#include "Tools.h"

using namespace std;

void Tools::createMarker(int id) {
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    cv::Mat markerImage;
    cv::aruco::drawMarker(dictionary, id, 400, markerImage, 1);
    cv::imwrite("../ArUco/marker" + std::to_string(id) + ".png", markerImage);
}

float Tools::euclideanDist(cv::Point2i &a, cv::Point2i &b) {
    cv::Point2f diff = a - b;
    float dist = cv::sqrt(diff.x * diff.x + diff.y * diff.y);
    //cout << "Distance between two point: " << dist << endl;
    return dist;
}

void Tools::warpImage(const cv::Mat &background, const cv::Mat &src, cv::Mat &dst, cv::Point2f location[]) {

    //input point
    cv::Point2f inPoints[4];
    inPoints[0] = cv::Point2i(0, 0);
    inPoints[1] = cv::Point2i(0, src.rows);
    inPoints[2] = cv::Point2i(src.cols, src.rows);
    inPoints[3] = cv::Point2i(src.cols, 0);

    cv::Mat matrix = cv::getPerspectiveTransform(inPoints, location);
    cv::warpPerspective(src, dst, matrix, dst.size());

    for (int i = 0; i < dst.cols; i++) {
        for (int j = 0; j < dst.rows; j++) {
            if (dst.at<cv::Vec3b>(cv::Point(i, j))[0] <= 5
                && dst.at<cv::Vec3b>(cv::Point(i, j))[1] <= 5
                && dst.at<cv::Vec3b>(cv::Point(i, j))[2] <= 5) {

                dst.at<cv::Vec3b>(cv::Point(i, j)) = background.at<cv::Vec3b>(cv::Point(i, j));
            }
        }
    }
}


void Tools::overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                         cv::Mat &output, cv::Point2i location) {

    background.copyTo(output);

    // start at the row indicated by location, or at row 0 if location.y is negative.
    for (int y = std::max(location.y, 0); y < background.rows; ++y) {
        int fY = y - location.y; // because of the translation

        // we are done of we have processed all rows of the foreground image.
        if (fY >= foreground.rows)
            break;

        // start at the column indicated by location,

        // or at column 0 if location.x is negative.
        for (int x = std::max(location.x, 0); x < background.cols; ++x) {
            int fX = x - location.x; // because of the translation.

            // we are done with this row if the column is outside the foreground image.
            if (fX >= foreground.cols)
                break;

            // determine the opacity of the foreground pixel, using its fourth (alpha) channel.
            double opacity = ((double) foreground.data[fY * foreground.step + fX * foreground.channels() + 3]) / 255.;

            // and now combine the background and foreground pixel, using the opacity,
            // but only if opacity > 0.
            for (int c = 0; opacity > 0 && c < output.channels(); ++c) {
                unsigned char foregroundPx =
                        foreground.data[fY * foreground.step + fX * foreground.channels() + c];
                unsigned char backgroundPx =
                        background.data[y * background.step + x * background.channels() + c];
                output.data[y * output.step + output.channels() * x + c] =
                        backgroundPx * (1. - opacity) + foregroundPx * opacity;
            }
        }
    }
}

void Tools::warpOverlayImage(const cv::Mat &background, const cv::Mat &foreground, cv::Mat &output,
                             std::vector<cv::Point2f> location) {

    //input point
    cv::Point2f inPointsD[4];
    inPointsD[0] = cv::Point2i(0, foreground.rows);
    inPointsD[1] = cv::Point2i(foreground.cols, foreground.rows);
    inPointsD[2] = cv::Point2i(foreground.cols, 0);
    inPointsD[3] = cv::Point2i(0, 0);


    int minX = std::min(std::min(location[0].x, location[3].x),
                        std::min(location[1].x, location[2].x));

    int maxX = std::max(std::max(location[1].x, location[2].x),
                        std::max(location[0].x, location[3].x));

    int minY = std::min(std::min(location[0].y, location[1].y),
                        std::min(location[2].y, location[3].y));


    int maxY = std::max(std::max(location[2].y, location[3].y),
                        std::max(location[0].y, location[1].y));

    // Print coordinates of the marker
    /*cout << "Marker coordinates top left :" << location[0].x << " " << location[0].y << endl;
    cout << "Marker coordinates top right :" << location[1].x << " " << location[1].y << endl;
    cout << "Marker coordinates bottom right :" << location[2].x << " " << location[2].y << endl;
    cout << "Marker coordinates bottom left :" << location[3].x << " " << location[3].y << endl;*/


    // Print maxX and maxY and minX and minY
    /*cout << "Max X: " << maxX << endl;
    cout << "Min X: " << minX << endl;
    cout << "Max Y: " << maxY << endl;
    cout << "Min Y: " << minY << endl;*/

    int offsetX = maxX - minX;
    int offsetY = maxY - minY;


    //Print offset
    /*cout << "Offset X: " << offsetX << endl;
    cout << "Offset Y: " << offsetY << endl;*/

    //destination points
    cv::Point2f outPointsD[4];
    outPointsD[0] = location[2]; // TL
    outPointsD[1] = location[3]; // TR
    outPointsD[2] = location[0]; // BR
    outPointsD[3] = location[1]; // BL

    cv::Mat matrixD = cv::getPerspectiveTransform(inPointsD, outPointsD);
    cv::Mat dinoDst = cv::Mat::zeros(output.rows, output.cols, foreground.type());
    cv::warpPerspective(foreground, dinoDst, matrixD, dinoDst.size());

    // Setup a rectangle to define your region of interest
    cv::Rect myROI = cv::Rect(minX, minY, offsetX, offsetY);

    // Crop the full image to that image contained by the rectangle myROI
    // Note that this doesn't copy the data
    cv::Mat croppedImage = dinoDst(myROI);

    //cv::imshow("Test", croppedImage);

    Tools::overlayImage(output, croppedImage, output, location[0]);
}

