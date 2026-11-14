#include "Detector.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    const string source = argc > 1 && string(argv[1]) != "--cube"
                              ? argv[1] : HOMEWORK_VIDEO_PATH;
    Detector detector;

    const size_t dot = source.find_last_of('.');
    const string extension = dot < source.size() ? source.substr(dot) : "";
    if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" ||
        extension == ".bmp") {
        Mat image = imread(source);
        if (image.empty()) {
            cerr << "Cannot open image: " << source << '\n';
            return 1;
        }
        Mat result;
        detector.detect(image, result, true);
        imshow("Detection", result);
        waitKey(0);
        return 0;
    }

    VideoCapture capture(source);
    if (!capture.isOpened()) {
        cerr << "Cannot open input: " << source << '\n';
        return 1;
    }

    Mat frame;
    Mat result;
    while (capture.read(frame)) {
        detector.detect(frame, result, true);
        imshow("Detection", result);
        if (waitKey(1) == 27) {
            break;
        }
    }
    return 0;
}
