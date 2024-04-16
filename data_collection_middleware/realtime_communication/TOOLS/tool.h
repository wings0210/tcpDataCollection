#ifndef TOOL_H
#define TOOL_H
#include"TOOLS/struct.h"
#include <iostream>
#include <vector>
#include <QThread>
#include <cstdint>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <random>
#include <netinet/in.h>
#include <ctime>
#include <chrono>
#include <cmath>
class Tool
{
public:
    Tool();
    ~Tool();
    static std::vector<uint8_t> transfer(message msg,int flag);
    static std::vector<uint8_t> structToBytes(message data,int flag);
    static recieve_authentication reverse_transfer_authentication(uint8_t (&buffer)[1024],ssize_t bytesRead );
    static recieve_realtime_message reverse_transfer_realtime(uint8_t (&buffer)[1024],ssize_t bytesRead);
    static uint16_t generateRandomUInt16();
    static void timestampToByteArray(uint64_t timestamp, uint8_t byteArray[8]);
    static uint64_t get_timestamp();
    static std::vector<uint8_t> get_time(int type);
    static std::vector<uint8_t> get_location(locations loc);
    static std::vector<uint8_t> get_self_defining_data(locations loc);

    static uint16_t getTotalUnionSize(int flag,message_body& obj);


    static void insert_select(int a[],int n);
    static void binary_insert(int a[],int n);
};


















#endif
