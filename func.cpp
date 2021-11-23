#include "func.h"

struct handle_data {
    unsigned long process_id; // not used
    const char *process_name;
    HWND window_handle;
};


BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam) {
    handle_data& data = *(handle_data*)lParam;
    char buffer[128];
    int written = GetWindowTextA(handle, buffer, 128);
    if (written && strstr(buffer, data.process_name) != NULL) {
        data.window_handle = handle;
        return FALSE;
    }
    return TRUE;
}

HWND find_main_window(const char* process_name) {
    handle_data data;
    data.process_name = process_name;
    data.window_handle = 0;
    EnumWindows(enum_windows_callback, (LPARAM)&data);
    return data.window_handle;
}

bool WindowToMaterial(HWND hwnd, cv::Mat &aMaterial) {
    HDC aWindowDC = GetWindowDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(aWindowDC);
    if (!hwindowCompatibleDC) {
        return false;
    }

    if (!SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR)) {
        DeleteDC(hwindowCompatibleDC);
        return false;
    }

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    HBITMAP hbwindow = CreateCompatibleBitmap(aWindowDC, windowsize.right, windowsize.bottom);
    if (!hbwindow) {
        DeleteDC(hwindowCompatibleDC);
        return false;
    }

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = windowsize.right;
    bi.biHeight = -windowsize.bottom;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    if (!SelectObject(hwindowCompatibleDC, hbwindow)) {
        DeleteObject(hbwindow);
        DeleteDC(hwindowCompatibleDC);
        return false;
    }

    if (!StretchBlt(hwindowCompatibleDC, 0, 0, windowsize.right, windowsize.bottom, aWindowDC, 0, 0, windowsize.right, windowsize.bottom, SRCCOPY)) {
        DeleteObject(hbwindow);
        DeleteDC(hwindowCompatibleDC);
        return false;
    }

    aMaterial.release();
    aMaterial.create(windowsize.bottom, windowsize.right, CV_8UC4);

    if (!GetDIBits(aWindowDC, hbwindow, 0, windowsize.bottom, aMaterial.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS)) {
        aMaterial.release();
        DeleteObject(hbwindow);
        DeleteDC(hwindowCompatibleDC);
        return false;
    }
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);

    const bool success = aMaterial.cols > 0 && aMaterial.rows > 0;
    if (!success) {
        aMaterial.release();
    }
    return success;
}