#ifndef TCPER_H
#define TCPER_H
#include <string>
#include <queue>
#include<QDateTime>
#include <chrono>
#include <QDebug>
#include <QString>
#include <functional>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QProcess>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <QThread>
#include <cstdint>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include"TOOLS/tool.h"
#include"TOOLS/struct.h"
#include <QCoreApplication>
#include <QProcess>
#include <QStringList>
#include <yaml-cpp/yaml.h>
#include <sstream>


//线程1，定时发送心跳报文
//线程2, 定时发送实时信息
//线程3. 实时检测网络状态，准备断线重连
//线程4, ros订阅处理线程
//线程5, ros转发中转线程
//线程6，TCP报文接收线程




class Tcper
{
public:

    const char* server_ip;
    int  server_port;
    int  sockfd;


    Tcper();
    ~Tcper();

    void init();
    void connect_tcp(const char* server_ip, int server_port);
    void reconnect_tcp(int sockfd);
    void recieve(uint8_t (&buffer)[1024],ssize_t bytesRead);
    void timing_response();
    void communicate(int sockfd);
    void disconnect_tcp();


    void keep_alive();
    void init_message();
    message get_authentication();
    void get_realtime_message(locations loc);
    void writeToBuffer(message msg);
    void get_heartbeat_msg();
    void timing_upload();
    void inspect_connect();


    void inspect_buffer();
    int flag=1;           //union使用标志位，1表示鉴权body信息，2表示实时body信息，3表示实时回应信息
    message msg;
    std::vector<uint8_t> new_msg;

    heartbeat_message heart_msg;
    std::vector<uint8_t> heart_vec;
    message realtime_msg;

    //待发送数据队列
    static std::queue<std::vector<uint8_t>> buffer;
    //存放订阅ROS话题的定位数据队列
    static std::queue<locations> buffer_queue;

     int net_flag=0;

     static int error;
     static socklen_t len;


private:
    const int MAX_BUFFER_SIZE = 1024;
    const int RECONNECT_INTERVAL = 5; // 重连间隔时间（秒）

};

/*
class Mythread:public QThread,public Tcper
{
public:
    void run(){
      Tcper::timing_upload(Tcper::sockfd);

    }
};
*/












#endif
