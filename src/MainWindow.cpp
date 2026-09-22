#include "MainWindow.h"

#include <QLabel>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Video Browser");
    resize(1000, 700);

    setStyleSheet(
        "QMainWindow { background-color: #202124; color: #eeeeee; }"
        "QLabel { color: #eeeeee; padding: 24px; }"
    );

    const QStringList missing = findMissingDependencies();

    auto *label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);

    if (missing.isEmpty()) {
        label->setText(
            "<h1>Video Browser</h1>"
            "<p>Initial application build successful.</p>"
            "<p>VLC, FFmpeg, and FFprobe were found.</p>"
        );
    } else {
        QString message =
            "<h1>Video Browser</h1>"
            "<p>The application is running, but these dependencies are missing:</p>"
            "<ul>";

        for (const QString &dependency : missing) {
            message += "<li>" + dependency.toHtmlEscaped() + "</li>";
        }

        message +=
            "</ul>"
            "<p>Install them with:</p>"
            "<pre>sudo apt install vlc ffmpeg</pre>";

        label->setText(message);
    }

    setCentralWidget(label);
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
