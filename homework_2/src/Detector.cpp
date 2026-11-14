#include "Detector.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>

#include <string>
#include <vector>

using namespace std;
using namespace aruco;

void detectAprilTags(const Mat& input, Mat& output) {
    static ArucoDetector detector(getPredefinedDictionary(DICT_APRILTAG_36h11));
    vector<vector<Point2f>> corners;
    vector<int> ids;
    detector.detectMarkers(input, corners, ids);

    for (size_t i = 0; i < ids.size(); ++i) {
        const auto& points = corners[i];
        for (size_t j = 0; j < points.size(); ++j) {
            line(output, points[j], points[(j + 1) % points.size()],
                 Scalar(0, 255, 0), 2);
        }
        putText(output, "AprilTag " + to_string(ids[i]), points[0],
                FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
    }
}

void detectCubeColor(const Mat& input, Mat& output) {
    const Rect region(input.cols / 4, input.rows / 4,
                      input.cols / 2, input.rows * 3 / 10);
    Mat hsv;
    cvtColor(input(region), hsv, COLOR_BGR2HSV);

    struct ColorRange {
        const char* name;
        Scalar lower;
        Scalar upper;
        Scalar display;
    };
    const ColorRange colors[] = {
        {"Red",    {0, 110, 70},   {9, 255, 255},   {0, 0, 255}},
        {"Orange", {10, 100, 70},  {22, 255, 255},  {0, 140, 255}},
        {"Yellow", {23, 100, 80},  {38, 255, 255},  {0, 255, 255}},
        {"Green",  {39, 70, 50},   {89, 255, 255},  {0, 255, 0}},
        {"Blue",   {90, 70, 40},   {135, 255, 255}, {255, 0, 0}},
        {"White",  {0, 0, 205},    {179, 70, 255}, {255, 255, 255}},
        {"Red",    {170, 110, 70}, {179, 255, 255}, {0, 0, 255}}
    };

    const ColorRange* best = nullptr;
    int best_count = 0;
    for (const auto& color : colors) {
        Mat mask;
        inRange(hsv, color.lower, color.upper, mask);
        const int count = countNonZero(mask);
        if (count > best_count) {
            best_count = count;
            best = &color;
        }
    }

    if (best == nullptr || best_count < region.area() / 20) {
        return;
    }

    rectangle(output, Rect(12, 12, 265, 48), Scalar(0, 0, 0), FILLED);
    rectangle(output, Rect(20, 20, 32, 32), best->display, FILLED);
    putText(output, string("Cube: ") + best->name, Point(65, 47),
            FONT_HERSHEY_SIMPLEX, 0.75, Scalar(255, 255, 255), 2);
}
