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

### Completed
- Project builds successfully locally.
- App launches and runs locally.
- Thumbnail generation is implemented using `ffmpeg`.
- Thumbnail filenames now use SHA1 hashes of video paths to avoid collisions.
- Thumbnail output directories are created automatically before generating thumbnails.
- Thumbnail queue advances reliably after success or failure.
- Full video paths are stored in each list item for thumbnail matching.
- Video list is set to `QListView::ListMode`.
- Video scanning now searches `QDir::homePath()` recursively for common video formats.

### Fixed Recently
- Fixed the `QProcess::ExitStatus` include issue.
- Replaced `generateThumbnail()` calls with `thumbnailGenerator->generate()`.
- Removed the duplicate/incomplete `scanVideoDirectories()` block.
- Restored directory scanning in `refreshVideos()`.

### Current Issue
- Only 1 thumbnail is visible out of 3 test videos.
- Git-based compile failed.
- Work is paused for now.

### Possible Causes Under Review
- Thumbnail assignment to list items.
- UI refresh after thumbnail generation.
- Queue processing for all videos.
- Thumbnail path uniqueness and file creation success.
- Build/configuration differences in the Git environment.

### Build status
latest build https://github.com/Joecephus2/video-browser/actions/runs/36042960723/job/107779386358 failed


### Exact build error

/usr/include/x86_64-linux-gnu/qt6/QtCore/qdiriterator.h:25:5: note:   candidate expects 3 arguments, 4 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qdiriterator.h:23:5: note: candidate: ‘QDirIterator::QDirIterator(const QString&, IteratorFlags)’
   23 |     QDirIterator(const QString &path,
      |     ^~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qdiriterator.h:23:5: note:   candidate expects 2 arguments, 4 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qdiriterator.h:22:5: note: candidate: ‘QDirIterator::QDirIterator(const QDir&, IteratorFlags)’
   22 |     QDirIterator(const QDir &dir, IteratorFlags flags = NoIteratorFlags);
      |     ^~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qdiriterator.h:22:5: note:   candidate expects 2 arguments, 4 provided
ninja: build stopped: subcommand failed.
Error: Process completed with exit code 1.
