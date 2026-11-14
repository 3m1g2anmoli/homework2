#pragma once

#include <opencv2/core.hpp>

using namespace cv;

void detectAprilTags(const Mat& input, Mat& output);
void detectCubeColor(const Mat& input, Mat& output, int saturation_min,
                     int white_value_min);

class Detector {
public:
    void setColorThresholds(int saturation_min, int white_value_min) {
        saturation_min_ = saturation_min;
        white_value_min_ = white_value_min;
    }

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
            detectCubeColor(input, output, saturation_min_, white_value_min_);
        }
    }

private:
    int saturation_min_ = 70;
    int white_value_min_ = 205;
};
