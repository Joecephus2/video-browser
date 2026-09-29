#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>
#include <QString>

class ThumbnailGenerator : public QObject
{
    Q_OBJECT

public:
    explicit ThumbnailGenerator(QObject *parent = nullptr);
    void generate(const QString &videoPath, const QString &thumbnailPath, qint64 timestampMilliseconds);

signals:
    void thumbnailReady(const QString &videoPath, const QString &thumbnailPath);
    void thumbnailFailed(const QString &videoPath, const QString &errorMessage);
};

#endif // THUMBNAILGENERATOR_H
