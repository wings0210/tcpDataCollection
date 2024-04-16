#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcper=new Tcper();

    tcper->connect_tcp(tcper->server_ip,tcper->server_port);
    tcper->init();
    tcper->inspect_connect();
    //timer(tcper);


}

MainWindow::~MainWindow()
{
    if(tcper!=nullptr)
        delete tcper;
    delete ui;
}

void MainWindow::timer(Tcper *tcper){
    std::thread timing_thread([=]{
        tcper->communicate(tcper->sockfd);
    });
    timing_thread.detach();
}
