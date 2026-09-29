#include "MainWindow.h"
#include "ThumbnailGenerator.h"

#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QUrl>
#include <QVBoxLayout>
#include <QIcon>

static const QStringList videoExtensions = {
    "*.mp4", "*.mkv", "*.avi", "*.mov", "*.webm", "*.flv"
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      searchBox(new QLineEdit(this)),
      videoList(new QTreeWidget(this)),
      thumbnailGenerator(new ThumbnailGenerator(this))
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QPushButton *chooseButton = new QPushButton("Choose Folder", this);
    QPushButton *refreshButton = new QPushButton("Refresh", this);

    topLayout->addWidget(searchBox);
    topLayout->addWidget(chooseButton);
    topLayout->addWidget(refreshButton);

    videoList->setColumnCount(2);
    videoList->setHeaderLabels({ "Name", "Type" });
    videoList->setSelectionMode(QAbstractItemView::SingleSelection);
    videoList->setExpandsOnDoubleClick(true);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(videoList);
    setCentralWidget(central);

    connect(chooseButton, &QPushButton::clicked,
            this, &MainWindow::chooseDirectory);
    connect(refreshButton, &QPushButton::clicked,
            this, &MainWindow::refreshVideos);
    connect(searchBox, &QLineEdit::textChanged,
            this, &MainWindow::filterVideos);
    connect(videoList, &QTreeWidget::itemDoubleClicked,
            this, &MainWindow::handleItemDoubleClicked);

    connect(thumbnailGenerator, &ThumbnailGenerator::thumbnailReady,
            this, &MainWindow::handleThumbnailReady);
    connect(thumbnailGenerator, &ThumbnailGenerator::thumbnailFailed,
            this, &MainWindow::handleThumbnailFailed);

    loadConfiguration();
    if (scanRootPath.isEmpty()) {
        promptForScanFolder();
    }
    refreshVideos();
}

void MainWindow::chooseDirectory()
{
    promptForScanFolder();
    refreshVideos();
}

void MainWindow::refreshVideos()
{
    videoList->clear();

    if (scanRootPath.isEmpty()) {
        return;
    }

    scanVideoDirectories();
    videoList->expandAll();
}

void MainWindow::filterVideos(const QString &text)
{
    const QString filter = text.trimmed().toLower();

    QTreeWidgetItemIterator it(videoList);
    while (*it) {
        QTreeWidgetItem *item = *it;
        bool visible = true;

        if (!filter.isEmpty()) {
            const QString name = item->text(0).toLower();
            const QString path = item->data(0, Qt::UserRole).toString().toLower();
            visible = name.contains(filter) || path.contains(filter);
        }

        item->setHidden(!visible);
        ++it;
    }
}

void MainWindow::handleItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    if (!item) {
        return;
    }

    const QString filePath = item->data(0, Qt::UserRole).toString();
    if (filePath.isEmpty()) {
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void MainWindow::loadConfiguration()
{
    QSettings settings("video-browser", "video-browser");
    scanRootPath = settings.value("scanRoot").toString();
}

void MainWindow::saveConfiguration() const
{
    QSettings settings("video-browser", "video-browser");
    settings.setValue("scanRoot", scanRootPath);
}

void MainWindow::promptForScanFolder()
{
    const QString folder = QFileDialog::getExistingDirectory(
        this,
        "Choose video folder",
        QDir::homePath()
    );

    if (!folder.isEmpty()) {
        scanRootPath = folder;
        saveConfiguration();
    }
}

void MainWindow::scanVideoDirectories()
{
    addFolderItems(nullptr, scanRootPath);
}

void MainWindow::addFolderItems(QTreeWidgetItem *parentItem, const QString &folderPath)
{
    QDir dir(folderPath);

    QFileInfoList entries = dir.entryInfoList(
        QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
        QDir::DirsFirst | QDir::Name
    );

    QTreeWidgetItem *folderItem = parentItem;

    if (parentItem == nullptr) {
        folderItem = new QTreeWidgetItem(videoList);
        folderItem->setText(0, QFileInfo(folderPath).fileName());
        folderItem->setText(1, "Folder");
        folderItem->setData(0, Qt::UserRole, folderPath);
    }

    for (const QFileInfo &info : entries) {
        if (info.isDir()) {
            QTreeWidgetItem *childFolder = new QTreeWidgetItem(folderItem);
            childFolder->setText(0, info.fileName());
            childFolder->setText(1, "Folder");
            childFolder->setData(0, Qt::UserRole, info.absoluteFilePath());

            addFolderItems(childFolder, info.absoluteFilePath());
        } else if (videoExtensions.contains("*" + info.suffix().toLower())) {
            QTreeWidgetItem *videoItem = new QTreeWidgetItem(folderItem);
            videoItem->setText(0, info.fileName());
            videoItem->setText(1, "Video");
            videoItem->setData(0, Qt::UserRole, info.absoluteFilePath());
            videoItem->setIcon(0, QIcon());

            queueThumbnail(info.absoluteFilePath());
        }
    }
}

QString MainWindow::thumbnailPathForVideo(const QString &videoPath) const
{
    QFileInfo info(videoPath);
    return info.absolutePath() + "/" + info.completeBaseName() + ".jpg";
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
    if (pendingThumbnailVideos.isEmpty()) {
        return;
    }

    const QString videoPath = pendingThumbnailVideos.head();
    const QString thumbPath = thumbnailPathForVideo(videoPath);

    thumbnailGenerator->generate(videoPath, thumbPath, 10000);
}

void MainWindow::handleThumbnailReady(const QString &videoPath,
                                      const QString &thumbnailPath)
{
    if (!pendingThumbnailVideos.isEmpty()) {
        pendingThumbnailVideos.dequeue();
    }

    thumbnailPaths[videoPath] = thumbnailPath;

    QTreeWidgetItemIterator it(videoList);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if (item->data(0, Qt::UserRole).toString() == videoPath) {
            item->setIcon(0, QIcon(thumbnailPath));
            break;
        }
        ++it;
    }

    startNextThumbnail();
}

void MainWindow::handleThumbnailFailed(const QString &videoPath,
                                       const QString &errorMessage)
{
    Q_UNUSED(videoPath);
    Q_UNUSED(errorMessage);

    if (!pendingThumbnailVideos.isEmpty()) {
        pendingThumbnailVideos.dequeue();
    }

    startNextThumbnail();
}
