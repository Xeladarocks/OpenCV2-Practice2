#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>

#include "func.h"

/** Function Headers */
void detect(cv::Mat frame);

/** Global variables */
cv::String upper_body_cascade_name = "C:\\dev\\c++\\OpenCV2-Test\\haarcascade_upperbody.xml";
cv::CascadeClassifier upper_body_cascade;
std::string window_name = "Capture - Upper Body detection";
cv::RNG rng(12345);

/** @function main */
int main(int argc, const char** argv) {

    cv::VideoCapture capture(0, cv::CAP_DSHOW); // camera
    cv::Mat frame;

    /*// HWND hwndWindow = getDesktopWindow();
    const char* AppName = "Chrome";
    HWND hwndWindow = find_main_window(AppName); // window
    SetForegroundWindow(hwndWindow);*/

    if (!upper_body_cascade.load(upper_body_cascade_name)){ printf("--(!)Error loading\n"); return -1; };

    int key = 0;
    while(key != 27) {
        //WindowToMaterial(hwndWindow, frame)
        capture.read(frame);
        if(frame.empty()) {
            std::cout << "frame is empty" << std::endl;
        } else {
            detect(frame);
            imshow("output", frame);
        }

        key = cv::waitKey(60);
    }

    /*    //-- 2. Read the video stream
    if (capture.isOpened()) {
        while (true) {
            capture >> frame;

            //-- 3. Apply the classifier to the frame
            if (!frame.empty()) {
                detectAndDisplay(frame);
            } else {
                printf(" --(!) No captured frame -- Break!"); break;
            }

            int c = waitKey(10);
            if ((char)c == 'c') { break; }
        }
    }*/
    cv::destroyAllWindows();
    return 0;
}

/** @function detectAndDisplay */
void detect(cv::Mat frame) {
    std::vector<cv::Rect> bodies;
    cv::Mat frame_gray;

    cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    //-- Detect faces
    upper_body_cascade.detectMultiScale(frame_gray, bodies, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    for (size_t i = 0; i < bodies.size(); i++) {
        rectangle(frame, bodies[i], cv::Scalar(255, 0, 255));
    }
}