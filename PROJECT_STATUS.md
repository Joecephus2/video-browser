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
- [ ] Thumbnail generation
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

- GitHub Actions result: FAIL
- Workflow run: https://github.com/Joecephus2/video-browser/actions/runs/35934499543
- Failed job: Add thumbnail generator
- Failed step: build

### Exact build error

/usr/include/c++/13/format:2093:22:   required from ‘typename std::basic_format_context<_Out, _CharT>::iterator std::formatter<_Tp, _CharT>::format(_Tp, std::basic_format_context<_Out, _CharT>&) const [with _Out = std::__format::_Sink_iter<char>; _Tp = float; _CharT = char; typename std::basic_format_context<_Out, _CharT>::iterator = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator]’
/usr/include/c++/13/format:3659:41:   required from ‘std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(size_t)::<lambda(auto:50&)> [with auto:50 = float]’
/usr/include/c++/13/format:3145:44:   required from ‘decltype(auto) std::basic_format_arg<_Context>::_M_visit(_Visitor&&, std::__format::_Arg_t) [with _Visitor = std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(size_t)::<lambda(auto:50&)>; _Context = std::basic_format_context<std::__format::_Sink_iter<char>, char>]’
/usr/include/c++/13/format:3192:28:   required from ‘decltype(auto) std::visit_format_arg(_Visitor&&, basic_format_arg<_Context>) [with _Visitor = __format::_Formatting_scanner<__format::_Sink_iter<char>, char>::_M_format_arg(size_t)::<lambda(auto:50&)>; _Context = basic_format_context<__format::_Sink_iter<char>, char>]’
/usr/include/c++/13/format:3648:23:   required from ‘constexpr void std::__format::_Formatting_scanner<_Out, _CharT>::_M_format_arg(size_t) [with _Out = std::__format::_Sink_iter<char>; _CharT = char; size_t = long unsigned int]’
/usr/include/c++/13/format:3643:7:   required from here
/usr/include/c++/13/format:1772:17: error: ‘class std::__cxx11::basic_string<char>’ has no member named ‘resize’
 1772 |           __str.resize(__n);
      |           ~~~~~~^~~~~~
/usr/include/c++/13/format:1773:17: error: ‘class std::__cxx11::basic_string<char>’ has no member named ‘resize’
 1773 |           __str.resize(__f(__str.data(), __n));
      |           ~~~~~~^~~~~~
ninja: build stopped: subcommand failed.
Error: Process completed with exit code 1.
