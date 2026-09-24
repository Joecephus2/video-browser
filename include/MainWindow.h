#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QMainWindow>
#include <QQueue>
#include <QStringList>

class QListWidget;
class QLineEdit;
class ThumbnailGenerator;

class MainWindow final : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStringList findMissingDependencies() const;

    void loadConfiguration();
    void saveConfiguration() const;
    void scanVideoDirectories();
    void refreshVideos();
    void chooseDirectory();
    void filterVideos(const QString &text);

    QString thumbnailPathForVideo(const QString &videoPath) const;
    void queueThumbnail(const QString &videoPath);
    void startNextThumbnail();
    void handleThumbnailReady(const QString &videoPath,
                              const QString &thumbnailPath);
    void handleThumbnailFailed(const QString &videoPath,
                               const QString &errorMessage);

    QStringList videoDirectories;
    QListWidget *videoList = nullptr;
    QLineEdit *searchBox = nullptr;
    ThumbnailGenerator *thumbnailGenerator = nullptr;

    QQueue<QString> pendingThumbnailVideos;
    QHash<QString, QString> thumbnailPaths;
};

#endif
