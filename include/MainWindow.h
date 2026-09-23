#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QListWidget;

class QListWidget;
class QLineEdit;

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

    QStringList videoDirectories;
    QListWidget *videoList = nullptr;
    QLineEdit *searchBox = nullptr;
};

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
