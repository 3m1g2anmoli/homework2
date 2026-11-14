#include "Detector.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    const string choice = argc > 1 ? argv[1] : "april";
    const bool cube_mode = choice == "cube";
    const string source = cube_mode ? CUBE_VIDEO_PATH
                        : choice == "april" || choice == "snapshot"
                        ? APRIL_VIDEO_PATH : choice;

    VideoCapture capture(source);
    if (!capture.isOpened()) {
        cerr << "Cannot open video: " << source << '\n';
        return 1;
    }

    Detector detector;
    int binary_threshold = 128;
    int saturation_min = 70;
    int white_value_min = 205;

    namedWindow("Image_raw", WINDOW_NORMAL);
    namedWindow("Image_binary", WINDOW_NORMAL);
    namedWindow("result", WINDOW_NORMAL);
    resizeWindow("Image_raw", 400, 225);
    resizeWindow("Image_binary", 400, 225);
    resizeWindow("result", 400, 225);
    moveWindow("Image_raw", 0, 0);
    moveWindow("Image_binary", 420, 0);
    moveWindow("result", 840, 0);
    createTrackbar("Threshold", "Image_binary", nullptr, 255);
    createTrackbar("HSV S min", "result", nullptr, 255);
    createTrackbar("White V min", "result", nullptr, 255);
    setTrackbarPos("Threshold", "Image_binary", binary_threshold);
    setTrackbarPos("HSV S min", "result", saturation_min);
    setTrackbarPos("White V min", "result", white_value_min);

    Mat frame;
    while (capture.read(frame)) {
        binary_threshold = getTrackbarPos("Threshold", "Image_binary");
        saturation_min = getTrackbarPos("HSV S min", "result");
        white_value_min = getTrackbarPos("White V min", "result");
        Mat gray;
        Mat binary;
        Mat result;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        threshold(gray, binary, binary_threshold, 255, THRESH_BINARY);

        detector.setColorThresholds(saturation_min, white_value_min);
        if (cube_mode) {
            detector.detect(frame, result, true);
        } else {
            detector.detect(frame, result);
        }

        rectangle(result, Rect(8, result.rows - 42, 520, 34),
                  Scalar(0, 0, 0), FILLED);
        putText(result,
                "Binary: " + to_string(binary_threshold) +
                "  HSV S: " + to_string(saturation_min) +
                "  White V: " + to_string(white_value_min),
                Point(16, result.rows - 18), FONT_HERSHEY_SIMPLEX, 0.55,
                Scalar(255, 255, 255), 2);

        imshow("Image_raw", frame);
        imshow("Image_binary", binary);
        imshow("result", result);

        const int key = waitKey(33);
        if (key == 27) {
            break;
        }
        if (key == 's' || key == 'S' || choice == "snapshot") {
            imwrite("Image_raw.png", frame);
            imwrite("Image_binary.png", binary);
            imwrite("result.png", result);

            Mat raw_small;
            Mat binary_small;
            Mat binary_color;
            Mat result_small;
            resize(frame, raw_small, Size(640, 360));
            resize(binary, binary_small, Size(640, 360));
            cvtColor(binary_small, binary_color, COLOR_GRAY2BGR);
            resize(result, result_small, Size(640, 360));
            rectangle(raw_small, Rect(0, 0, 150, 28), Scalar(0, 0, 0), FILLED);
            rectangle(binary_color, Rect(0, 0, 170, 28), Scalar(0, 0, 0), FILLED);
            rectangle(result_small, Rect(0, 0, 110, 28), Scalar(0, 0, 0), FILLED);
            putText(raw_small, "Image_raw", Point(8, 20),
                    FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 2);
            putText(binary_color, "Image_binary", Point(8, 20),
                    FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 2);
            putText(result_small, "result", Point(8, 20),
                    FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 2);
            rectangle(result_small, Rect(0, 330, 640, 30),
                      Scalar(0, 0, 0), FILLED);
            putText(result_small,
                    "T=" + to_string(binary_threshold) +
                    " S=" + to_string(saturation_min) +
                    " V=" + to_string(white_value_min),
                    Point(8, 350), FONT_HERSHEY_SIMPLEX, 0.65,
                    Scalar(255, 255, 255), 2);
            Mat screenshot;
            vector<Mat> panels = {raw_small, binary_color, result_small};
            hconcat(panels, screenshot);
            imwrite("screenshot.png", screenshot);
            if (choice == "snapshot") {
                break;
            }
        }
    }
    return 0;
}
