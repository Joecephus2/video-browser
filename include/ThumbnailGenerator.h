#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>
#include <QProcess>
#include <QString>

class ThumbnailGenerator : public QObject
{
    Q_OBJECT

public:
    explicit ThumbnailGenerator(QObject *parent = nullptr);

    void generate(const QString &videoPath,
                  const QString &thumbnailPath,
                  qint64 timestampMilliseconds);

signals:
    void thumbnailReady(const QString &videoPath, const QString &thumbnailPath);
    void thumbnailFailed(const QString &videoPath, const QString &errorMessage);

private slots:
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *process_;
    QString currentVideoPath_;
    QString currentThumbnailPath_;
};

#endif // THUMBNAILGENERATOR_H
