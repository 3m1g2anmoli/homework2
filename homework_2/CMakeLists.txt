if(NOT OpenCV_DIR AND EXISTS "D:/OPENCV/opencv/build/OpenCVConfig.cmake")
    set(OpenCV_DIR "D:/OPENCV/opencv/build")
endif()

find_package(OpenCV REQUIRED COMPONENTS core imgproc objdetect videoio highgui imgcodecs)

add_executable(homework_2 src/main.cpp src/Detector.cpp)
target_compile_features(homework_2 PRIVATE cxx_std_17)
target_include_directories(homework_2 PRIVATE include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(homework_2 PRIVATE ${OpenCV_LIBS})
target_compile_definitions(homework_2 PRIVATE
    HOMEWORK_VIDEO_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../video.mp4")

if(WIN32 AND OpenCV_DIR)
    set(_opencv_bin "${OpenCV_DIR}/x64/vc16/bin")
    if(EXISTS "${_opencv_bin}")
        add_custom_command(TARGET homework_2 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_opencv_bin}/opencv_world510$<$<CONFIG:Debug>:d>.dll"
                "${_opencv_bin}/opencv_videoio_ffmpeg510_64.dll"
                "$<TARGET_FILE_DIR:homework_2>"
            COMMENT "Copying OpenCV runtime libraries")
    endif()
endif()
