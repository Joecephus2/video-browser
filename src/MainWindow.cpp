#include "MainWindow.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>
#include <QStatusBar>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Video Browser");
    resize(1000, 700);

    setStyleSheet(
        "QMainWindow { background-color: #202124; color: #eeeeee; }"
        "QWidget { background-color: #202124; color: #eeeeee; }"
        "QLabel { color: #eeeeee; padding: 12px; }"
        "QLineEdit {"
        "  background-color: #292a2d;"
        "  color: #eeeeee;"
        "  border: 1px solid #444444;"
        "  padding: 8px;"
        "  font-size: 14px;"
        "}"
        "QPushButton {"
        "  background-color: #405d80;"
        "  color: #ffffff;"
        "  border: 1px solid #5579a3;"
        "  padding: 8px 14px;"
        "}"
        "QPushButton:hover { background-color: #4f7198; }"
        "QListWidget {"
        "  background-color: #292a2d;"
        "  color: #eeeeee;"
        "  border: 1px solid #444444;"
        "  font-size: 14px;"
        "}"
        "QListWidget::item { padding: 8px; }"
        "QListWidget::item:selected { background-color: #405d80; }"
    );

    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);

    auto *title = new QLabel("<h1>Video Browser</h1>", centralWidget);
    title->setTextFormat(Qt::RichText);

    searchBox = new QLineEdit(centralWidget);
    searchBox->setPlaceholderText("Search videos...");

    auto *controlsLayout = new QHBoxLayout();

    auto *chooseFolderButton =
        new QPushButton("Choose Video Folder", centralWidget);

    auto *refreshButton =
        new QPushButton("Refresh", centralWidget);

    controlsLayout->addWidget(chooseFolderButton);
    controlsLayout->addWidget(refreshButton);
    controlsLayout->addStretch();

    videoList = new QListWidget(centralWidget);
    videoList->setAlternatingRowColors(true);

    layout->addWidget(title);
    layout->addWidget(searchBox);
    layout->addLayout(controlsLayout);
    layout->addWidget(videoList);

    setCentralWidget(centralWidget);

    connect(
        chooseFolderButton,
        &QPushButton::clicked,
        this,
        &MainWindow::chooseDirectory
    );

    connect(
        refreshButton,
        &QPushButton::clicked,
        this,
        &MainWindow::refreshVideos
    );

    connect(
        searchBox,
        &QLineEdit::textChanged,
        this,
        &MainWindow::filterVideos
    );

    connect(
        videoList,
        &QListWidget::itemDoubleClicked,
        this,
        [this](QListWidgetItem *item) {
            if (!item) {
                return;
            }

            const QString filePath =
                item->data(Qt::UserRole).toString();

            if (filePath.isEmpty()) {
                return;
            }

            if (!QProcess::startDetached("vlc", {filePath})) {
                QMessageBox::warning(
                    this,
                    "Unable to Play Video",
                    "Could not start VLC."
                );
            }
        }
    );

    loadConfiguration();
    refreshVideos();

    const QStringList missing = findMissingDependencies();

    if (!missing.isEmpty()) {
        const QString message =
            "Missing dependencies: " + missing.join(", ") +
            "\n\nInstall them with:\n"
            "sudo apt install vlc ffmpeg";

        statusBar()->showMessage(message);
    }
}

QStringList MainWindow::findMissingDependencies() const
{
    QStringList missing;

    const QStringList requiredPrograms = {
        "vlc",
        "ffmpeg",
        "ffprobe"
    };

    for (const QString &program : requiredPrograms) {
        if (QStandardPaths::findExecutable(program).isEmpty()) {
            missing.append(program);
        }
    }

    return missing;
}

void MainWindow::loadConfiguration()
{
    const QString configDirectory =
        QStandardPaths::writableLocation(
            QStandardPaths::AppConfigLocation
        );

    QDir().mkpath(configDirectory);

    const QString configPath = configDirectory + "/config.json";

    QFile configFile(configPath);

    if (!configFile.exists()) {
        videoDirectories.append(
            QDir::homePath() + "/VideoBrowserTest"
        );

        saveConfiguration();
        return;
    }

    if (!configFile.open(QIODevice::ReadOnly)) {
        videoDirectories.append(
            QDir::homePath() + "/VideoBrowserTest"
        );

        return;
    }

    const QJsonDocument document =
        QJsonDocument::fromJson(configFile.readAll());

    configFile.close();

    if (!document.isObject()) {
        videoDirectories.append(
            QDir::homePath() + "/VideoBrowserTest"
        );

        return;
    }

    const QJsonArray directories =
        document.object()["video_directories"].toArray();

    for (const QJsonValue &value : directories) {
        if (value.isString()) {
            QString path = value.toString();

            if (path.startsWith("~/")) {
                path = QDir::homePath() + path.mid(1);
            }

            const QString cleanPath = QDir::cleanPath(path);

            if (!videoDirectories.contains(cleanPath)) {
                videoDirectories.append(cleanPath);
            }
        }
    }

    if (videoDirectories.isEmpty()) {
        videoDirectories.append(
            QDir::homePath() + "/VideoBrowserTest"
        );
    }
}

void MainWindow::saveConfiguration() const
{
    const QString configDirectory =
        QStandardPaths::writableLocation(
            QStandardPaths::AppConfigLocation
        );

    QDir().mkpath(configDirectory);

    const QString configPath = configDirectory + "/config.json";

    QJsonArray directories;

    for (const QString &directory : videoDirectories) {
        directories.append(directory);
    }

    QJsonObject configObject;
    configObject["video_directories"] = directories;

    QFile configFile(configPath);

    if (configFile.open(QIODevice::WriteOnly)) {
        configFile.write(
            QJsonDocument(configObject).toJson(
                QJsonDocument::Indented
            )
        );

        configFile.close();
    }
}

void MainWindow::chooseDirectory()
{
    const QString directory =
        QFileDialog::getExistingDirectory(
            this,
            "Choose Video Folder",
            QDir::homePath()
        );

    if (directory.isEmpty()) {
        return;
    }

    const QString cleanDirectory = QDir::cleanPath(directory);

    if (!videoDirectories.contains(cleanDirectory)) {
        videoDirectories.append(cleanDirectory);
        saveConfiguration();
    }

    refreshVideos();
}

void MainWindow::refreshVideos()
{
    videoList->clear();
    scanVideoDirectories();

    filterVideos(searchBox->text());
}

void MainWindow::filterVideos(const QString &text)
{
    const QString searchText = text.trimmed();
    int visibleCount = 0;

    for (int index = 0; index < videoList->count(); ++index) {
        QListWidgetItem *item = videoList->item(index);

        const bool matches =
            item->text().contains(
                searchText,
                Qt::CaseInsensitive
            );

        item->setHidden(!matches);

        if (matches) {
            ++visibleCount;
        }
    }

    statusBar()->showMessage(
        QString("Showing %1 of %2 video file(s)")
            .arg(visibleCount)
            .arg(videoList->count())
    );
}

void MainWindow::scanVideoDirectories()
{
    const QStringList videoExtensions = {
        "mp4",
        "mkv",
        "avi",
        "mov",
        "wmv",
        "webm",
        "m4v",
        "mpg",
        "mpeg",
        "ts",
        "m2ts",
        "flv",
        "3gp"
    };

    for (const QString &directory : videoDirectories) {
        if (!QDir(directory).exists()) {
            continue;
        }

        QDirIterator iterator(
            directory,
            QDir::Files | QDir::Readable,
            QDirIterator::Subdirectories
        );

        while (iterator.hasNext()) {
            const QString filePath = iterator.next();
            const QFileInfo fileInfo(filePath);

            if (videoExtensions.contains(
                    fileInfo.suffix().toLower())) {
                auto *item =
                    new QListWidgetItem(fileInfo.fileName());

                item->setData(
                    Qt::UserRole,
                    fileInfo.absoluteFilePath()
                );

                videoList->addItem(item);
            }
        }
    }
}
