#ifndef OPENCV2_TEST_FUNC_H
#define OPENCV2_TEST_FUNC_H


#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>

bool WindowToMaterial(HWND hwnd, cv::Mat &aMaterial);
HWND find_main_window(const char* process_name);


#endif OPENCV2_TEST_FUNC_H
