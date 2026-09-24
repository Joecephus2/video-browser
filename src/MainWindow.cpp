#include "MainWindow.h"
#include "ThumbnailGenerator.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QIcon>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QListView>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      searchBox(new QLineEdit(this)),
      videoList(new QListWidget(this)),
      thumbnailGenerator(new ThumbnailGenerator(this))
{
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    layout->addWidget(searchBox);
    layout->addWidget(videoList);
    setCentralWidget(central);

    videoList->setViewMode(QListView::ListMode);

    connect(searchBox, &QLineEdit::textChanged,
            this, &MainWindow::filterVideos);

    connect(thumbnailGenerator, &ThumbnailGenerator::thumbnailReady,
            this, &MainWindow::handleThumbnailReady);

    connect(thumbnailGenerator, &ThumbnailGenerator::thumbnailFailed,
            this, &MainWindow::handleThumbnailFailed);

    refreshVideos();
}

void MainWindow::chooseDirectory()
{
}

void MainWindow::refreshVideos()
{
    videoList->clear();
    pendingThumbnailVideos.clear();
    thumbnailPaths.clear();
    scanVideoDirectories();
}

void MainWindow::filterVideos(const QString &text)
{
    for (int i = 0; i < videoList->count(); ++i) {
        QListWidgetItem *item = videoList->item(i);
        item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));
    }
}

void MainWindow::handleThumbnailReady(const QString &videoPath,
                                      const QString &thumbnailPath)
{
    pendingThumbnailVideos.removeOne(videoPath);

    for (int i = 0; i < videoList->count(); ++i) {
        QListWidgetItem *item = videoList->item(i);
        if (item->data(Qt::UserRole).toString() == videoPath) {
            item->setIcon(QIcon(thumbnailPath));
            break;
        }
    }

    startNextThumbnail();
}

void MainWindow::handleThumbnailFailed(const QString &videoPath,
                                       const QString &errorMessage)
{
    qWarning() << "Thumbnail failed for" << videoPath << ":" << errorMessage;

    pendingThumbnailVideos.removeOne(videoPath);
    startNextThumbnail();
}

void MainWindow::loadConfiguration()
{
}

void MainWindow::saveConfiguration() const
{
}

void MainWindow::scanVideoDirectories()
{
    const QStringList videoExtensions = {
        "*.mp4", "*.mkv", "*.avi", "*.mov", "*.wmv",
        "*.webm", "*.m4v", "*.mpg", "*.mpeg", "*.ts",
        "*.m2ts", "*.flv", "*.3gp"
    };

    const QStringList searchDirs = {
        QDir::homePath()
    };

    QDirIterator it(searchDirs,
                    videoExtensions,
                    QDir::Files,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        const QString videoPath = it.next();
        auto *item = new QListWidgetItem(QFileInfo(videoPath).fileName());
        item->setData(Qt::UserRole, videoPath);
        videoList->addItem(item);
        queueThumbnail(videoPath);
    }
}

QString MainWindow::thumbnailPathForVideo(const QString &videoPath) const
{
    const QByteArray hash =
        QCryptographicHash::hash(videoPath.toUtf8(), QCryptographicHash::Sha1).toHex();

    return QDir::homePath() + "/.video-browser/thumbnails/" +
           QString::fromUtf8(hash) + ".jpg";
}

void MainWindow::queueThumbnail(const QString &videoPath)
{
    if (!thumbnailPaths.contains(videoPath)) {
        thumbnailPaths.insert(videoPath, thumbnailPathForVideo(videoPath));
    }

    if (!pendingThumbnailVideos.contains(videoPath)) {
        pendingThumbnailVideos.enqueue(videoPath);
    }

    if (pendingThumbnailVideos.size() == 1) {
        startNextThumbnail();
    }
}

void MainWindow::startNextThumbnail()
{
    if (pendingThumbnailVideos.isEmpty()) {
        return;
    }

    const QString videoPath = pendingThumbnailVideos.head();
    const QString thumbnailPath = thumbnailPaths.value(videoPath);

    thumbnailGenerator->generate(videoPath, thumbnailPath, 10000);
}
