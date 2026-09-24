#include "ThumbnailGenerator.h"

#include <QProcess>

ThumbnailGenerator::ThumbnailGenerator(QObject *parent)
    : QObject(parent),
      process_(new QProcess(this))
{
    connect(process_,
            &QProcess::finished,
            this,
            &ThumbnailGenerator::processFinished);
}

void ThumbnailGenerator::generate(const QString &videoPath,
                                  const QString &thumbnailPath,
                                  qint64 timestampMilliseconds)
{
    if (videoPath.isEmpty() || thumbnailPath.isEmpty()) {
        emit thumbnailFailed(videoPath, "Video or thumbnail path is empty.");
        return;
    }

    if (process_->state() != QProcess::NotRunning) {
        emit thumbnailFailed(videoPath,
                             "A thumbnail generation process is already running.");
        return;
    }

    currentVideoPath_ = videoPath;
    currentThumbnailPath_ = thumbnailPath;

    const double timestampSeconds =
        static_cast<double>(timestampMilliseconds) / 1000.0;

    const QStringList arguments{
        "-y",
        "-ss",
        QString::number(timestampSeconds, 'f', 3),
        "-i",
        videoPath,
        "-frames:v",
        "1",
        "-q:v",
        "2",
        thumbnailPath
    };

    process_->start("ffmpeg", arguments);

    if (!process_->waitForStarted(1000)) {
        emit thumbnailFailed(videoPath,
                             "Could not start ffmpeg: " +
                                 process_->errorString());

        currentVideoPath_.clear();
        currentThumbnailPath_.clear();
    }
}

void ThumbnailGenerator::processFinished(
    int exitCode,
    QProcess::ExitStatus exitStatus)
{
    const QString videoPath = currentVideoPath_;
    const QString thumbnailPath = currentThumbnailPath_;

    const bool success =
        exitStatus == QProcess::NormalExit &&
        exitCode == 0;

    if (success) {
        emit thumbnailReady(videoPath, thumbnailPath);
    } else {
        QString errorMessage =
            process_->readAllStandardError().trimmed();

        if (errorMessage.isEmpty()) {
            errorMessage = "ffmpeg failed to generate the thumbnail.";
        }

        emit thumbnailFailed(videoPath, errorMessage);
    }

    currentVideoPath_.clear();
    currentThumbnailPath_.clear();
}
