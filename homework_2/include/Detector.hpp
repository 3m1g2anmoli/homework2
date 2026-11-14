#pragma once

#include <opencv2/core.hpp>

using namespace cv;

void detectAprilTags(const Mat& input, Mat& output);
void detectCubeColor(const Mat& input, Mat& output);

class Detector {
public:
    void detect(const Mat& input, Mat& output) {
        detect(input, output, false);
    }

    void detect(const Mat& input, Mat& output, bool detect_cube_color) {
        if (input.empty()) {
            output.release();
            return;
        }

        input.copyTo(output);
        detectAprilTags(input, output);
        if (detect_cube_color) {
            detectCubeColor(input, output);
        }
    }
};
