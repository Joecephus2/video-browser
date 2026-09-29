cmake_minimum_required(VERSION 3.16)

project(video-browser VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(video-browser
    src/main.cpp
    src/MainWindow.cpp
    src/ThumbnailGenerator.cpp
)

target_include_directories(video-browser PRIVATE include)
target_link_libraries(video-browser PRIVATE Qt6::Widgets)
