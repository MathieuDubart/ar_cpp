#include <iostream>
#include <algorithm>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>

#include "Tools.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv) {


    cv::Mat markerImage, landscape, imgIn, imageCopy, dinosaur, alien, ovni, skin, ash;

    cv::VideoCapture inputVideo;
    inputVideo.open(0);
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::namedWindow("Out", 0);

    //Tools::createMarker(1);
    //Tools::createMarker(2);
    //Tools::createMarker(3);
    //Tools::createMarker(4);
    //Tools::createMarker(5);
    //Tools::createMarker(6);
    //Tools::createMarker(7);
    //Tools::createMarker(8);
    //Tools::createMarker(9);

    landscape = imread("../Resources/landscape.jpg");
    dinosaur = imread("../Resources/dinosaur.png", IMREAD_UNCHANGED);
    alien = imread("../Resources/alien.png", IMREAD_UNCHANGED);
    ash = imread("../Resources/ash.png", IMREAD_UNCHANGED);
    ovni = imread("../Resources/ufo.png", IMREAD_UNCHANGED);
    skin = imread("../Resources/skin.png", IMREAD_UNCHANGED);

    //cv::VideoCapture capOut("../sample.mp4", 2200);
    //capOut>>landscape;

    //cv::VideoCapture gifCapOut("some.gif", CAP_FFMPEG)
    //gifCapOut>>gif;

    while (inputVideo.grab()) {
        inputVideo.retrieve(imgIn);
        imgIn.copyTo(imageCopy);

        //capOut>>landscape;

        cv::Mat frame = imgIn.clone();

        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners;
        cv::aruco::detectMarkers(imgIn, dictionary, markerCorners, markerIds);
        std::vector<cv::Point2i> ptsOut(16);

        bool showDino = false;
        bool showAlien = false;
        bool showOvni = false;
        bool showSkin = false;

        Point2i dinoLoc;
        int dinoCornerId;

        Point2i alienLoc;
        int alienCornerId;

        Point2i skinLoc;
        int skinCornerId;

        Point2i ovniLoc;
        int ovniCornerId;

        try {
            // if at least one marker detected
            if (!markerIds.empty()) {
                cv::aruco::drawDetectedMarkers(imgIn, markerCorners, markerIds);

                int cnt = 0;

                for (int i = 0; i < (int) markerIds.size(); i++) {


                    if (markerIds[i] == 6) {
                        cnt++;
                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ptsOut[0] = cv::Point2i(x, y);

                    }
                    else if (markerIds[i] == 7) {
                        cnt++;
                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ptsOut[1] = cv::Point2i(x, y);

                    }
                    else if (markerIds[i] == 8) {
                        cnt++;
                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ptsOut[2] = cv::Point2i(x, y);

                    }
                    else if (markerIds[i] == 9) {
                        cnt++;
                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ptsOut[3] = cv::Point2i(x, y);
                    }
                    else if (markerIds[i] == 5) {
                        showDino = true;

                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        dinoCornerId = i;
                        dinoLoc = cv::Point2i(x, y);
                    }
                    else if (markerIds[i] == 4) {
                        showAlien = true;

                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        alienCornerId = i;
                        alienLoc = cv::Point2i(x, y);
                    }
                    else if (markerIds[i] == 3) {
                        showOvni = true;

                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        ovniCornerId = i;
                        ovniLoc = cv::Point2i(x, y);

                    }
                    else if (markerIds[i] == 2) {
                        showSkin = true;

                        int x = (int) (markerCorners[i][0].x + markerCorners[i][2].x) / 2;
                        int y = (int) (markerCorners[i][0].y + markerCorners[i][2].y) / 2;

                        skinCornerId = i;
                        skinLoc = cv::Point2i(x, y);

                    }
                }

                if (cnt == 4) {
                    //destination points
                    cv::Point2f outPoints[4];
                    outPoints[0] = ptsOut[0];
                    outPoints[1] = ptsOut[1];
                    outPoints[2] = ptsOut[2];
                    outPoints[3] = ptsOut[3];

                    Tools::warpImage(frame, landscape, imgIn, outPoints);
                }

                if(showDino) {
                    Tools::warpOverlayImage(imgIn, dinosaur, imgIn, markerCorners[dinoCornerId]);
                }

                if(showAlien) {
                    Tools::warpOverlayImage(imgIn, alien, imgIn, markerCorners[alienCornerId]);
                }

                if(showSkin) {
                    Tools::warpOverlayImage(imgIn, skin, imgIn, markerCorners[skinCornerId]);
                }

                if(showOvni && ovniLoc.x < ptsOut[2].x && ovniLoc.x > ptsOut[0].x) {


                    if (ovniLoc.y < ptsOut[2].y && ovniLoc.y > ptsOut[0].y) {
                        Tools::warpOverlayImage(imgIn, ovni, imgIn, markerCorners[ovniCornerId]);
                    }
                }

                if(showAlien && showDino) {
                    double distc = Tools::euclideanDist(dinoLoc, alienLoc);
                    if(distc < 200) {
                        alien = ash;
                    }
                }

            }

        } catch (const std::exception &e) {
            cout << endl << " e : " << e.what() << endl;
            cout << "Could not do warpPerspective !! " << endl;
        }

        cv::imshow("Out", imgIn);
        char key = (char) cv::waitKey(40);
        if (key == 27)
            break;
    }

    return 0;
}
