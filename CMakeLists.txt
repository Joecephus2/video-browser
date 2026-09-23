cmake_minimum_required(VERSION 3.20)

project(video-browser
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(video-browser
    src/main.cpp
    src/MainWindow.cpp
    src/ThumbnailGenerator.cpp
    include/MainWindow.h
    include/ThumbnailGenerator.h
)

target_include_directories(video-browser PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(video-browser PRIVATE
    Qt6::Widgets
)

enable_testing()
