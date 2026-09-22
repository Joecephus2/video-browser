#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    application.setApplicationName("Video Browser");
    application.setApplicationVersion("0.1.0");
    application.setOrganizationName("Video Browser Project");

    MainWindow window;
    window.show();

    return application.exec();
}
