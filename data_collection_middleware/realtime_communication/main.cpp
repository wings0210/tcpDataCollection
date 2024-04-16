#include "mainwindow.h"
#include <QApplication>
#include"ROS/roser.h"
int main(int argc, char **argv)
{

    QApplication a(argc, argv);

    Roser roser(argc,argv);

    MainWindow w;
    w.show();
    return a.exec();
}
