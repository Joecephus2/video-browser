#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>
#include <QHash>

class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
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
    void handleItemDoubleClicked(QTreeWidgetItem *item, int column);
    void handleThumbnailReady(const QString &videoPath, const QString &thumbnailPath);
    void handleThumbnailFailed(const QString &videoPath, const QString &errorMessage);

private:
    void loadConfiguration();
    void saveConfiguration() const;
    void promptForScanFolder();
    void scanVideoDirectories();
    void addFolderItems(QTreeWidgetItem *parentItem, const QString &folderPath);
    QString thumbnailPathForVideo(const QString &videoPath) const;
    void queueThumbnail(const QString &videoPath);
    void startNextThumbnail();

    QLineEdit *searchBox;
    QTreeWidget *videoList;
    ThumbnailGenerator *thumbnailGenerator;

    QString scanRootPath;
    QQueue<QString> pendingThumbnailVideos;
    QHash<QString, QString> thumbnailPaths;
};

#endif // MAINWINDOW_H
