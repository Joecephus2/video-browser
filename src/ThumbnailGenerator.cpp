#include "ThumbnailGenerator.h"

#include <QProcess>

ThumbnailGenerator::ThumbnailGenerator(QObject *parent)
    : QObject(parent)
{
}

void ThumbnailGenerator::generate(const QString &videoPath, const QString &thumbnailPath, qint64 timestampMilliseconds)
{
    auto *process = new QProcess(this);

    connect(process, &QProcess::finished, this, [this, process, videoPath, thumbnailPath](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit && exitCode == 0) {
            emit thumbnailReady(videoPath, thumbnailPath);
        } else {
            emit thumbnailFailed(videoPath, process->errorString());
        }
        process->deleteLater();
    });

    connect(process, &QProcess::errorOccurred, this, [this, process, videoPath](QProcess::ProcessError) {
        emit thumbnailFailed(videoPath, process->errorString());
    });

    process->setProgram("ffmpeg");
    process->setArguments({
        "-y",
        "-ss", QString::number(timestampMilliseconds / 1000.0),
        "-i", videoPath,
        "-frames:v", "1",
        "-q:v", "2",
        thumbnailPath
    });
    process->start();
}
