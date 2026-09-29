#include "MainWindow.h"
#include "ThumbnailGenerator.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QIcon>
#include <QLineEdit>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QSettings>
#include <QDebug>
#include <QAbstractItemView>
#include <QTreeWidgetItemIterator>
#include <QVBoxLayout>
#include <QWidget>

static bool isVideoFile(const QString &fileName)
{
    static const QStringList extensions = {
        ".mp4", ".mkv", ".avi", ".mov", ".webm", ".flv"
    };

    QString lower = fileName.toLower();
    for (const QString &ext : extensions) {
        if (lower.endsWith(ext))
            return true;
    }
    return false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      searchBox(new QLineEdit(this)),
      videoList(new QTreeWidget(this)),
      thumbnailGenerator(new ThumbnailGenerator(this))
{
    setWindowTitle("Video Browser");

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    searchBox->setPlaceholderText("Filter videos...");
    layout->addWidget(searchBox);
    layout->addWidget(videoList);
    setCentralWidget(central);

    videoList->setColumnCount(1);
    videoList->setHeaderHidden(true);
    videoList->setSelectionMode(QAbstractItemView::SingleSelection);
    videoList->setUniformRowHeights(true);

    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::filterVideos);
    connect(videoList, &QTreeWidget::itemDoubleClicked, this, &MainWindow::handleItemDoubleClicked);

    connect(thumbnailGenerator, &ThumbnailGenerator::thumbnailReady,
            this, &MainWindow::handleThumbnailReady);
    connect(thumbnailGenerator, &ThumbnailGenerator::thumbnailFailed,
            this, &MainWindow::handleThumbnailFailed);

    loadConfiguration();
    refreshVideos();
}

void MainWindow::loadConfiguration()
{
    QSettings settings("video-browser", "video-browser");
    scanRootPath = settings.value("scanRoot").toString();

    if (scanRootPath.isEmpty() || !QDir(scanRootPath).exists()) {
        promptForScanFolder();
    }
}

void MainWindow::saveConfiguration() const
{
    QSettings settings("video-browser", "video-browser");
    settings.setValue("scanRoot", scanRootPath);
}

void MainWindow::promptForScanFolder()
{
    QString folder = QFileDialog::getExistingDirectory(
        this,
        "Select Video Folder",
        QDir::homePath()
    );

    if (!folder.isEmpty()) {
        scanRootPath = folder;
        saveConfiguration();
    }
}

void MainWindow::refreshVideos()
{
    videoList->clear();

    if (scanRootPath.isEmpty())
        return;

    addFolderItems(nullptr, scanRootPath);
    videoList->expandAll();
}

void MainWindow::addFolderItems(QTreeWidgetItem *parentItem, const QString &folderPath)
{
    QDir dir(folderPath);
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

    QTreeWidgetItem *folderItem = parentItem;

    if (parentItem == nullptr) {
        folderItem = new QTreeWidgetItem(videoList);
        QString folderName = dir.dirName();
        if (folderName.isEmpty())
            folderName = folderPath;
        folderItem->setText(0, folderName);
        folderItem->setData(0, Qt::UserRole, folderPath);
        folderItem->setIcon(0, QIcon::fromTheme("folder"));
        folderItem->setExpanded(true);
    }

    for (const QFileInfo &info : entries) {
        if (info.isDir()) {
            QTreeWidgetItem *childFolder = new QTreeWidgetItem(folderItem);
            childFolder->setText(0, info.fileName());
            childFolder->setData(0, Qt::UserRole, info.absoluteFilePath());
            childFolder->setIcon(0, QIcon::fromTheme("folder"));
            addFolderItems(childFolder, info.absoluteFilePath());
        } else if (info.isFile() && isVideoFile(info.fileName())) {
            QTreeWidgetItem *videoItem = new QTreeWidgetItem(folderItem);
            videoItem->setText(0, info.fileName());
            videoItem->setData(0, Qt::UserRole, info.absoluteFilePath());
            videoItem->setIcon(0, QIcon::fromTheme("video-x-generic"));
            queueThumbnail(info.absoluteFilePath());
        }
    }
}

QString MainWindow::thumbnailPathForVideo(const QString &videoPath) const
{
    QString cacheDir = QDir::homePath() + "/.cache/video-browser-thumbnails";
    QDir().mkpath(cacheDir);

    QString safeName = videoPath;
    safeName.replace("/", "_");
    safeName.replace("\\", "_");
    safeName.replace(":", "_");

    return cacheDir + "/" + safeName + ".jpg";
}

void MainWindow::queueThumbnail(const QString &videoPath)
{
    pendingThumbnailVideos.enqueue(videoPath);
    if (pendingThumbnailVideos.size() == 1) {
        startNextThumbnail();
    }
}

void MainWindow::startNextThumbnail()
{
    if (pendingThumbnailVideos.isEmpty())
        return;

    QString videoPath = pendingThumbnailVideos.dequeue();
    QString thumbPath = thumbnailPathForVideo(videoPath);

    if (QFileInfo::exists(thumbPath)) {
        handleThumbnailReady(videoPath, thumbPath);
        startNextThumbnail();
        return;
    }

    thumbnailGenerator->generate(videoPath, thumbPath, 10000);
}

void MainWindow::handleThumbnailReady(const QString &videoPath, const QString &thumbnailPath)
{
    thumbnailPaths.insert(videoPath, thumbnailPath);

    for (int i = 0; i < videoList->topLevelItemCount(); ++i) {
        QTreeWidgetItem *top = videoList->topLevelItem(i);
        QTreeWidgetItemIterator it(top);
        while (*it) {
            QTreeWidgetItem *item = *it;
            if (item->data(0, Qt::UserRole).toString() == videoPath) {
                item->setIcon(0, QIcon(thumbnailPath));
                break;
            }
            ++it;
        }
    }

    startNextThumbnail();
}

void MainWindow::handleThumbnailFailed(const QString &videoPath, const QString &errorMessage)
{
    qWarning() << "Thumbnail failed for" << videoPath << ":" << errorMessage;
    startNextThumbnail();
}

void MainWindow::filterVideos(const QString &text)
{
    const QString needle = text.trimmed().toLower();

    for (int i = 0; i < videoList->topLevelItemCount(); ++i) {
        QTreeWidgetItem *top = videoList->topLevelItem(i);
        bool visible = false;

        QTreeWidgetItemIterator it(top);
        while (*it) {
            QTreeWidgetItem *item = *it;
            QString name = item->text(0).toLower();
            bool match = needle.isEmpty() || name.contains(needle);
            item->setHidden(!match);
            if (match)
                visible = true;
            ++it;
        }

        top->setHidden(!visible);
    }
}

void MainWindow::handleItemDoubleClicked(QTreeWidgetItem *item, int)
{
    if (!item)
        return;

    QString path = item->data(0, Qt::UserRole).toString();
    if (QFileInfo(path).isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
}

void MainWindow::chooseDirectory()
{
    promptForScanFolder();
    refreshVideos();
}
