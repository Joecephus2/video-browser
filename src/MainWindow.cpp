#include "MainWindow.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QProcess>
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

    videoList = new QListWidget(centralWidget);
    videoList->setAlternatingRowColors(true);

    layout->addWidget(title);
    layout->addWidget(videoList);

    setCentralWidget(centralWidget);

    loadConfiguration();
    scanVideoDirectories();

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

    const QStringList missing = findMissingDependencies();

    if (!missing.isEmpty()) {
        QString message =
            "Missing dependencies: " + missing.join(", ") +
            "\n\nInstall them with:\n"
            "sudo apt install vlc ffmpeg";

        statusBar()->showMessage(message);
    } else {
        statusBar()->showMessage(
            QString("Found %1 video file(s)").arg(videoList->count())
        );
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
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir().mkpath(configDirectory);

    const QString configPath = configDirectory + "/config.json";

    QFile configFile(configPath);

    if (!configFile.exists()) {
        QJsonObject configObject;
        QJsonArray directories;
        directories.append(QDir::homePath() + "/VideoBrowserTest");
        configObject["video_directories"] = directories;

        if (configFile.open(QIODevice::WriteOnly)) {
            configFile.write(
                QJsonDocument(configObject).toJson(QJsonDocument::Indented)
            );
            configFile.close();
        }

        videoDirectories.append(QDir::homePath() + "/VideoBrowserTest");
        return;
    }

    if (!configFile.open(QIODevice::ReadOnly)) {
        videoDirectories.append(QDir::homePath() + "/VideoBrowserTest");
        return;
    }

    const QJsonDocument document =
        QJsonDocument::fromJson(configFile.readAll());

    configFile.close();

    if (!document.isObject()) {
        videoDirectories.append(QDir::homePath() + "/VideoBrowserTest");
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

            videoDirectories.append(QDir::cleanPath(path));
        }
    }

    if (videoDirectories.isEmpty()) {
        videoDirectories.append(QDir::homePath() + "/VideoBrowserTest");
    }
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
                auto *item = new QListWidgetItem(fileInfo.fileName());

                item->setData(
                    Qt::UserRole,
                    fileInfo.absoluteFilePath()
                );

                videoList->addItem(item);
            }
        }
    }
}
