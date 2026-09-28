#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QQueue>
#include <QString>
#include <QSettings>

class QLineEdit;
class QListWidget;
class ThumbnailGenerator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void chooseDirectory();
    void refreshVideos();
    void filterVideos(const QString &text);

    void handleThumbnailReady(const QString &videoPath,
                              const QString &thumbnailPath);
    void handleThumbnailFailed(const QString &videoPath,
                               const QString &errorMessage);

private:
    void loadConfiguration();
    void saveConfiguration() const;
    void promptForScanFolder();
    void scanVideoDirectories();

    QString thumbnailPathForVideo(const QString &videoPath) const;
    void queueThumbnail(const QString &videoPath);
    void startNextThumbnail();

private:
    QLineEdit *searchBox;
    QListWidget *videoList;
    ThumbnailGenerator *thumbnailGenerator;

    QString scanRootPath;

    QQueue<QString> pendingThumbnailVideos;
    QHash<QString, QString> thumbnailPaths;
};

#endif // MAINWINDOW_H

