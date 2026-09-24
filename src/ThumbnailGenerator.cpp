#include "ThumbnailGenerator.h"

#include <QDir>
#include <QFileInfo>

ThumbnailGenerator::ThumbnailGenerator(QObject *parent)
    : QObject(parent),
      process_(new QProcess(this))
{
    connect(process_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &ThumbnailGenerator::processFinished);
}

void ThumbnailGenerator::generate(const QString &videoPath,
                                  const QString &thumbnailPath,
                                  qint64 timestampMilliseconds)
{
    currentVideoPath_ = videoPath;
    currentThumbnailPath_ = thumbnailPath;

    QDir().mkpath(QFileInfo(thumbnailPath).absolutePath());

    const QString timestampSeconds =
        QString::number(timestampMilliseconds / 1000.0, 'f', 3);

    process_->start("ffmpeg", {
        "-y",
        "-ss", timestampSeconds,
        "-i", videoPath,
        "-frames:v", "1",
        "-q:v", "2",
        thumbnailPath
    });
}

void ThumbnailGenerator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        emit thumbnailReady(currentVideoPath_, currentThumbnailPath_);
    } else {
        emit thumbnailFailed(currentVideoPath_, process_->readAllStandardError());
    }
}
