#ifndef STRUCT_H
#define STRUCT_H
#include <string>

#pragma pack(1)

struct authentication_message
{
    std::string   tag;                      //终端标识
    uint8_t       ctri_type;                //类型
    uint8_t       status;                   //状态标识
    uint8_t       terminal_code_length;     //终端条码长度
    //std::string   terminal_code;            //终端条码
    uint8_t       SIM_length;
    //std::string   SIM;
    uint8_t       ICCID_length;
    //std::string   ICCID;
    uint8_t       Vin_length;
    std::string   Vin;

};



//车辆状态
/*
0、无服务请求：0x0000
1、未准备完毕：0x0100
2、准备完毕：0x0200
3、正常行驶：0x0300
4、到达目的地：0x0400
5、服务失败（未到达目的地）：0x0500
6、车辆已暂停：0x0600
7、避障停车：0x0501
8、硬件失效：0x0520
9、定位失败：0x0510
10、偏移路线停止：0x0541
11、低矮障碍物停止：0x0531
12、车辆起步失败：0x1200
*/

struct DataTPL
{
    uint8_t       data_length_flag1;
    uint8_t       data_length_flag2;
    uint8_t       template_flag1;
    uint8_t       template_flag2;

    uint8_t       veh_status_high;        //车辆状态占用2字节
    uint8_t       veh_status_low;
    uint8_t       veh_auto_button;        //0,手动  1,自动
    uint16_t      trip_distance;       //站点总里程（最高千位，m）
    uint16_t      trip_left_mileage;   //站点剩余里程
    uint8_t       fusion_coordinate[10];
    uint16_t      current_speed;
    uint8_t       trip_current_id[4];
    uint8_t       trip_target_id[4];
    uint8_t       RTC_time[8];
};



struct realtime_message
{
    uint8_t       current_time[8];    //当前时间
    uint8_t       RTC_tag;
    uint8_t       RTC_time[8];        //数据采集时间
    uint8_t       location_tag;
    uint8_t       location[21];
    uint8_t       selfdefined_tag;    //自定义数据标识
    DataTPL       dataTPL;


};

struct responsing{
    uint16_t serial_numble;
    uint16_t id;
    uint8_t  result;
};

struct message_header
{
    uint16_t id;                        //消息id
    uint16_t attribute;                 //消息体属性
    uint16_t serial_numble;             //流水号

    //uint8_t package_numble;       //消息总包数，不分包不传
    //uint8_t package_id;           //消息包id，不分包不传
};


union message_body
{
    authentication_message *authentication;
    realtime_message       *realtime;
    responsing             *responing;
};

//公共消息主体格式
struct message
{
    uint8_t        identification_1;
    message_header header;
    message_body   body;
    uint8_t        checking_code;  //校验码
    uint8_t        identification_2;
};


struct recieve_realtime_message{
    uint8_t        identification_1;
    message_header header;
    uint16_t       serial_numble;
    uint16_t       id;
    uint8_t        result;
    uint8_t        checking_code;  //校验码
    uint8_t        identification_2;
};

struct recieve_authentication{
    uint8_t        identification_1;
    message_header header;
    uint16_t       serial_numble;
    uint8_t        result;
    uint8_t        RTC_time[8];
    uint8_t        checking_code;  //校验码
    uint8_t        identification_2;
};

struct heartbeat_message{
    uint8_t        identification_1;
    message_header header;
    uint8_t        checking_code;  //校验码
    uint8_t        identification_2;
};


struct locations
{
    //定位数据表
    double  longitude;
    double  latitude;
    double  azi;
    double  height;
    double  speed;
    double  accumulative_mileage;

    //自定义dataTPL
    int     veh_status;
    int     veh_auto_button;
    int     trip_distance;
    int     trip_left_mileage;
    char    trip_current_id[4];
    char    trip_target_id[4];
};


#pragma pack()


#endif
