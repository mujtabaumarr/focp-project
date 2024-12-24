cmake_minimum_required(VERSION 3.16)
project(HelloSFML)

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Use PkgConfig to find FFmpeg
find_package(PkgConfig REQUIRED)

# Manually set the FFmpeg paths if pkg-config doesn't find them
set(FFMPEG_INCLUDE_DIRS "/opt/homebrew/include")
set(FFMPEG_LIBRARIES "/opt/homebrew/lib/libavcodec.dylib;/opt/homebrew/lib/libavformat.dylib;/opt/homebrew/lib/libavutil.dylib;/opt/homebrew/lib/libswscale.dylib")

# Add OpenCV to the project
find_package(OpenCV REQUIRED)
# Manually set the OpenCV paths if not found automatically
set(OpenCV_DIR "/opt/homebrew/Cellar/opencv/4.x.x/share/OpenCV")  # Replace with your installed version path
set(OpenCV_INCLUDE_DIRS "/opt/homebrew/include/opencv4")
set(OpenCV_LIBS "/opt/homebrew/lib/libopencv_core.dylib;/opt/homebrew/lib/libopencv_highgui.dylib;/opt/homebrew/lib/libopencv_imgproc.dylib;/opt/homebrew/lib/libopencv_videoio.dylib")

# Add executable
add_executable(HelloSFML main.cpp)

# Link SFML libraries
target_link_libraries(HelloSFML sfml-graphics sfml-window sfml-system sfml-audio)

# Include FFmpeg directories
target_include_directories(HelloSFML PRIVATE ${FFMPEG_INCLUDE_DIRS})

# Link FFmpeg libraries
target_link_libraries(HelloSFML ${FFMPEG_LIBRARIES})

# Include OpenCV directories
target_include_directories(HelloSFML PRIVATE ${OpenCV_INCLUDE_DIRS})

# Link OpenCV libraries
target_link_libraries(HelloSFML ${OpenCV_LIBS})