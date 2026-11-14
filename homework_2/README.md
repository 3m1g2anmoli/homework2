# OpenCV homework 2

The program detects AprilTags in `homework_2/video.mp4` by default. Select `homework_2.exe` as the startup item in Visual Studio and press F5.

Three windows are shown: `Image_raw`, `Image_binary`, and `result`. The binary threshold can be adjusted in the `Image_binary` window. The HSV saturation and white value controls are in the `result` window. The current values are also printed on the result image.

Press `S` to save `Image_raw.png`, `Image_binary.png`, `result.png`, and a combined `screenshot.png` in the current working directory. Press Esc to stop playback.

Run with the `snapshot` command argument to save the first AprilTag frame and exit. An example is included as `screenshot.png`.

To process the Rubik's Cube video instead, set the Visual Studio command argument to `cube`. The program then reads the `video.mp4` in the project root and reports the visible face color. An explicit video path may also be passed to process an AprilTag video.
