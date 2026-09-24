# Video Browser Project Status

## Project goal

Native Qt 6 Linux desktop app for browsing and managing video files on Lubuntu/LXQt.

## Repository

- GitHub: https://github.com/Joecephus2/video-browser
- Active branch: main
- Last known good commit: [commit hash]
- Last attempted commit: [commit hash]
- Current version: [version]

## Build status

- GitHub Actions workflow: [workflow name]
- Last build result: PASS / FAIL
- Last successful commit: [commit hash]
- Latest failure: [short description]
- Full failure log: [link to Actions run]

## Implemented

- [x] Qt desktop window
- [x] Dark theme
- [x] Video directory configuration
- [x] Recursive video scanning
- [x] Search/filter
- [x] VLC double-click playback
- [ ] JSON media database
- [ ] Thumbnail generation integrated into the UI
- [ ] Duration extraction
- [ ] File-size overlay
- [ ] Hover preview
- [ ] Corrupt-video logging
- [ ] Database pruning
- [ ] First-run configuration
- [ ] Automated tests
- [ ] Release tarball

## Current task

Enable Thumbnail generation

### Next action

Retrieve the first source-file compiler error from the GitHub Actions build log. Then fix the thumbnail-generator compilation error before adding further features.


## Decisions

    UI framework: Qt 6 Widgets
    Language: C++20
    Metadata tools: FFmpeg/FFprobe
    Playback: VLC/libVLC
    Database format: JSON
    Build system: CMake
    CI: GitHub Actions
    Runtime target: Lubuntu/LXQt


## Known issues

    Cannot compile locally; GitHub Actions is the build environment.
    Artifact links may not expose compiler logs directly.
    Do not add a second CI workflow unless the existing one is unusable.

## Session checkpoint — 2026-09-23

### Current branch and commit

- Branch: `main`
- Latest commit: b9d4d288002c2db28fe15355b4efdc46d797f58c
- Commit message: Add thumbnail generator
- Working tree clean: YES 

### Current task

Add FFmpeg-based thumbnail generation using a new `ThumbnailGenerator` class.

### Changes made

- Added `PROJECT_STATUS.md` to track the project state, completed features, build status, current task, and next actions.
- Confirmed that the current `main` branch builds successfully before the latest thumbnail changes.
- Began the thumbnail-generation implementation.
- Added or prepared a `ThumbnailGenerator` component for FFmpeg-based thumbnail extraction.
- Added project-header includes using quotes, such as:
  `#include "ThumbnailGenerator.h"`
- Added Qt/library includes using angle brackets, such as:
  `#include <QProcess>`
- Added or updated thumbnail-related source files:
  - `include/ThumbnailGenerator.h`
  - `src/ThumbnailGenerator.cpp`
- Added or updated thumbnail-related test files:
  - `tests/ThumbnailGeneratorTests.cpp`
- Updated `CMakeLists.txt` to include the new thumbnail source and test files.
- Intended thumbnail behavior:
  - Extract a frame at approximately 10 seconds.
  - Use an earlier frame for videos shorter than 10 seconds.
  - Use FFmpeg through Qt's `QProcess`.
  - Avoid blocking the application interface.
  - Cache generated thumbnails for later reuse.
- Pushed the changes to the `main` branch.
- GitHub Actions failed while building the thumbnail-related changes.
- The exact compiler error still needs to be investigated.

### Build status
- Last known good commit: Unknown 
- Last attempted commit: 40c4ab588e15df38df0c7a990d8caac9f471fcc2
- Current version: Not yet assigned
- GitHub Actions workflow: Add thumbnail generator
- Last build result: FAIL
- Last successful commit: `
- Latest failure: Thumbnail generator compilation failure
- Full failure log: https://github.com/Joecephus2/video-browser/actions/runs/35938248074


### Exact build error

[3/6] Building CXX object CMakeFiles/video-browser.dir/src/ThumbnailGenerator.cpp.o
FAILED: [code=1] CMakeFiles/video-browser.dir/src/ThumbnailGenerator.cpp.o 
/usr/bin/c++ -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_WIDGETS_LIB -I/home/runner/work/video-browser/video-browser/build/video-browser_autogen/include -I/home/runner/work/video-browser/video-browser/include -isystem /usr/include/x86_64-linux-gnu/qt6/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt6 -isystem /usr/include/x86_64-linux-gnu/qt6/QtCore -isystem /usr/lib/x86_64-linux-gnu/qt6/mkspecs/linux-g++ -isystem /usr/include/x86_64-linux-gnu/qt6/QtGui -O3 -DNDEBUG -std=c++20 -fPIC -MD -MT CMakeFiles/video-browser.dir/src/ThumbnailGenerator.cpp.o -MF CMakeFiles/video-browser.dir/src/ThumbnailGenerator.cpp.o.d -o CMakeFiles/video-browser.dir/src/ThumbnailGenerator.cpp.o -c /home/runner/work/video-browser/video-browser/src/ThumbnailGenerator.cpp
In file included from /home/runner/work/video-browser/video-browser/src/ThumbnailGenerator.cpp:1:
/home/runner/work/video-browser/video-browser/include/ThumbnailGenerator.h:28:26: error: ‘QProcess::ExitStatus’ has not been declared
   28 |                          QProcess::ExitStatus exitStatus);
      |                          ^~~~~~~~
[4/6] Building CXX object CMakeFiles/video-browser.dir/src/main.cpp.o
[5/6] Building CXX object CMakeFiles/video-browser.dir/src/MainWindow.cpp.o
FAILED: [code=1] CMakeFiles/video-browser.dir/src/MainWindow.cpp.o 
/usr/bin/c++ -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_WIDGETS_LIB -I/home/runner/work/video-browser/video-browser/build/video-browser_autogen/include -I/home/runner/work/video-browser/video-browser/include -isystem /usr/include/x86_64-linux-gnu/qt6/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt6 -isystem /usr/include/x86_64-linux-gnu/qt6/QtCore -isystem /usr/lib/x86_64-linux-gnu/qt6/mkspecs/linux-g++ -isystem /usr/include/x86_64-linux-gnu/qt6/QtGui -O3 -DNDEBUG -std=c++20 -fPIC -MD -MT CMakeFiles/video-browser.dir/src/MainWindow.cpp.o -MF CMakeFiles/video-browser.dir/src/MainWindow.cpp.o.d -o CMakeFiles/video-browser.dir/src/MainWindow.cpp.o -c /home/runner/work/video-browser/video-browser/src/MainWindow.cpp
In file included from /home/runner/work/video-browser/video-browser/src/MainWindow.cpp:3:
/home/runner/work/video-browser/video-browser/include/ThumbnailGenerator.h:28:26: error: ‘QProcess::ExitStatus’ has not been declared
   28 |                          QProcess::ExitStatus exitStatus);
      |                          ^~~~~~~~
ninja: build stopped: subcommand failed.
Error: Process completed with exit code 1.

