#pragma once

#include <QObject>
#include <QString>

class QProcess;

class ThumbnailGenerator final : public QObject
{
    Q_OBJECT

public:
    explicit ThumbnailGenerator(QObject *parent = nullptr);

    void generate(const QString &videoPath,
                  const QString &thumbnailPath,
                  qint64 timestampMilliseconds = 10000);

signals:
    void thumbnailReady(const QString &videoPath,
                        const QString &thumbnailPath);

    void thumbnailFailed(const QString &videoPath,
                         const QString &errorMessage);

private slots:
    void processFinished(int exitCode,
                         QProcess::ExitStatus exitStatus);

private:
    QProcess *process_ = nullptr;
    QString currentVideoPath_;
    QString currentThumbnailPath_;
};
