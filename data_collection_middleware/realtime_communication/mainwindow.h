#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"TCP/tcper.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void timer(Tcper *tcper);

    Tcper *tcper;
    //Roser *roser;

private:
    Ui::MainWindow *ui;
};

class Mythread:public QThread
{
public:
    void run(){
      //Tcper::timing_upload(Tcper::sockfd);

    }
};

#endif // MAINWINDOW_H
