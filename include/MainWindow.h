#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QListWidget;

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

    QStringList videoDirectories;
    QListWidget *videoList = nullptr;
};

#endif
