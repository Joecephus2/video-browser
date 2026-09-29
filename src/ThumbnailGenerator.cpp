#include "ThumbnailGenerator.h"

#include <QFileInfo>
#include <QProcess>

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

    QStringList args;
    args << "-y";
    args << "-ss" << QString::number(timestampMilliseconds / 1000.0, 'f', 3);
    args << "-i" << videoPath;
    args << "-frames:v" << "1";
    args << "-q:v" << "2";
    args << thumbnailPath;

    process_->start("ffmpeg", args);
}

void ThumbnailGenerator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        emit thumbnailReady(currentVideoPath_, currentThumbnailPath_);
    } else {
        emit thumbnailFailed(currentVideoPath_, "ffmpeg failed");
    }
}
