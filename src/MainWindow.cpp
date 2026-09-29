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
