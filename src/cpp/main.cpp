#include <QtGui/QApplication>
#include "../headers/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Http Shark");
    a.setOrganizationName("VF");

    MainWindow w;

    w.setWindowTitle(a.applicationName());
    w.show();
    return a.exec();
}
