#include "D:\hasse\mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMouseTracking(true);
    w.show();
    return a.exec();
}
