#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("fabiorush92");
    QCoreApplication::setApplicationName("QIPTV");

    MainWindow w;
    w.show();

    return a.exec();
}
