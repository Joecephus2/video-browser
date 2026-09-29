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

## Project Status

# Project Status

## Completed
- Qt 6 project builds successfully
- `QDirIterator` issue fixed
- Main window compiles and launches
- Video scan root is now configurable
- App prompts for a scan folder on first run if none is saved
- Scan location is stored with `QSettings`
- Thumbnail generation system is wired into the UI

## Current Behavior
- App loads saved scan directory if available
- If no scan directory is configured, app prompts the user to choose one
- Videos are scanned recursively from the chosen folder
- Thumbnails are queued and generated one at a time

## Files Updated
- `include/MainWindow.h`
- `src/MainWindow.cpp`
- `src/main.cpp`

## Next Issue to Check
- Why only 1 of 3 thumbnails appears after the build passes
