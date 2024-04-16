#include"TCP/tcper.h"
std::queue<std::vector<uint8_t>> Tcper::buffer=std::queue<std::vector<uint8_t>>();
std::queue<locations> Tcper::buffer_queue=std::queue<locations>();
int Tcper::error=0;
socklen_t Tcper::len=sizeof(Tcper::error);
Tcper::Tcper(){
    this->server_ip="111.59.73.53";
    this->server_port=17058;



}
Tcper::~Tcper(){
    close(this->sockfd);

}

void Tcper::init(){

    inspect_buffer();
    this->timing_upload();
    communicate(this->sockfd);
    keep_alive();

}




//连接函数
void Tcper::connect_tcp(const char* server_ip, int server_port) {
    // 创建套接字
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        exit(1);
    }

    // 设置服务器地址和端口
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &(serverAddr.sin_addr)) <= 0) {
        std::cerr << "Invalid address." << std::endl;
        exit(1);
    }

    // 连接到服务器
    while(1){
        if (connect(this->sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Failed to connect to server." << std::endl;
            std::cout<<"继续尝试重连"<<std::endl;
            sleep(1);
            continue;
         }
        break;
    }
    std::cout << "Connected to server." << std::endl;
    std::vector<uint8_t> vec=Tool::transfer(get_authentication(),this->flag);
    ssize_t bytesSent = send(sockfd, vec.data(), vec.size(), 0);
    if (bytesSent == -1) {
         std::cerr << "Failed to send data." << std::endl;

    }
    std::cout << "发送鉴权信息成功，Sent " << bytesSent << " bytes of data." << std::endl;


}

//重连函数
void Tcper::reconnect_tcp(int sockfd) {
    close(sockfd);
    std::cout << "与服务器断开连接，重连中..." << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(RECONNECT_INTERVAL));
    this->connect_tcp(this->server_ip, this->server_port);
}

//主接收函数
void Tcper::communicate(int sockfd) {
    std::thread receive_thread([=](){
    uint8_t buffer[1024];
    while (true) {
    if(this->net_flag==0 && getsockopt(this->sockfd, SOL_SOCKET, SO_ERROR, &error, &len)==0){
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead == -1) {
            std::cerr << "Failed to receive data." << std::endl;

            reconnect_tcp(sockfd); // 调用reconnect函数进行重连
            continue;
        }
        if(bytesRead>0)
          recieve(buffer,bytesRead);

    }
    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }
  });
  receive_thread.detach();
}

//发送失败的要存在buffer里，待上线后续传
void Tcper::timing_upload(){
    std::thread realtime_upload([=]{
    while (true) {
    if(this->net_flag==0 && getsockopt(this->sockfd, SOL_SOCKET, SO_ERROR, &error, &len)==0){
        if(Tcper::buffer.empty()!=1){
        std::vector<uint8_t> new_msg=Tcper::buffer.front();
       // 发送数据
       //ssize_t bytesSent = send(sockfd, reinterpret_cast<char*>(&new_msg), sizeof(new_msg), 0);
        ssize_t bytesSent = send(this->sockfd, new_msg.data(), new_msg.size(), 0);
       if (bytesSent == -1) {
             std::cerr << "Failed to send data." << std::endl;
             break;
       }

       std::cout << "发送实时信息成功，Sent " << bytesSent << " bytes of data." << std::endl;

       // 暂停1秒
       std::this_thread::sleep_for(std::chrono::seconds(1));
       Tcper::buffer.pop();
       }

     }
    }
    });
    realtime_upload.detach();
}





message Tcper::get_authentication(){

    YAML::Node config = YAML::LoadFile("../config.yaml");
    std::string Vin = config["Vin"].as<std::string>();

    message msg1;
    msg1.identification_1=0x7e;
    msg1.identification_2=0x7e;
    msg1.header.id=0x0F01;
    msg1.header.serial_numble=Tool::generateRandomUInt16();


    msg1.body.authentication=new authentication_message;
    //this->flag=1;

    msg1.body.authentication->ctri_type=0x01;
    //msg1.body.authentication->ICCID="";
    msg1.body.authentication->ICCID_length=0x00;
    //msg1.body.authentication->SIM="";
    msg1.body.authentication->SIM_length=0x00;
    msg1.body.authentication->status=0x01;
    msg1.body.authentication->tag=Vin;
    //msg1.body.authentication->terminal_code="";
    msg1.body.authentication->terminal_code_length=0x00;
    msg1.body.authentication->Vin=Vin;
    msg1.body.authentication->Vin_length=msg1.body.authentication->Vin.length();

    msg1.header.attribute=Tool::getTotalUnionSize(this->flag,msg1.body);
    msg1.checking_code=0x00;

    //std::cout<<(msg1.body.authentication->Vin).size()<<std::endl;



    //std::cout<<sizeof(msg1)<<std::endl;
    return msg1;



}



void Tcper::get_realtime_message(locations loc){
    this->realtime_msg.identification_1=0x7e;
    this->realtime_msg.identification_2=0x7e;
    this->realtime_msg.header.id=0x0F02;
    this->realtime_msg.header.serial_numble=Tool::generateRandomUInt16();



    this->realtime_msg.body.realtime=new realtime_message;
    this->flag=2;

    std::vector<uint8_t> temp=Tool::get_time(2);
    this->realtime_msg.body.realtime->current_time[0]=temp[0];
    this->realtime_msg.body.realtime->current_time[1]=temp[1];
    this->realtime_msg.body.realtime->current_time[2]=temp[2];
    this->realtime_msg.body.realtime->current_time[3]=temp[3];
    this->realtime_msg.body.realtime->current_time[4]=temp[4];
    this->realtime_msg.body.realtime->current_time[5]=temp[5];
    this->realtime_msg.body.realtime->current_time[6]=temp[6];
    this->realtime_msg.body.realtime->current_time[7]=temp[7];

    this->realtime_msg.body.realtime->RTC_tag=0x01;
    std::vector<uint8_t> temp2=Tool::get_time(2);
    this->realtime_msg.body.realtime->RTC_time[0]=temp2[0];
    this->realtime_msg.body.realtime->RTC_time[1]=temp2[1];
    this->realtime_msg.body.realtime->RTC_time[2]=temp2[2];
    this->realtime_msg.body.realtime->RTC_time[3]=temp2[3];
    this->realtime_msg.body.realtime->RTC_time[4]=temp2[4];
    this->realtime_msg.body.realtime->RTC_time[5]=temp2[5];
    this->realtime_msg.body.realtime->RTC_time[6]=temp2[6];
    this->realtime_msg.body.realtime->RTC_time[7]=temp2[7];

    this->realtime_msg.body.realtime->location_tag=0x02;

    //后期引入接收的话题信息
    //locations loc={124.012346,24.987654,121.2,0,5,121.23};


    std::vector<uint8_t> temp3=Tool::get_location(loc);
    for(int i=0;i<21;i++){
    this->realtime_msg.body.realtime->location[i]=temp3[i];
    }

    this->realtime_msg.body.realtime->selfdefined_tag=0x03;

    std::vector<uint8_t> temp4=Tool::get_self_defining_data(loc);
    this->realtime_msg.body.realtime->dataTPL.data_length_flag1=0x00;
    this->realtime_msg.body.realtime->dataTPL.data_length_flag2=0x33;
    this->realtime_msg.body.realtime->dataTPL.template_flag1=0x01;
    this->realtime_msg.body.realtime->dataTPL.template_flag2=0x70;
    this->realtime_msg.body.realtime->dataTPL.veh_status_high=temp4[0];
    this->realtime_msg.body.realtime->dataTPL.veh_status_low=temp4[1];
    this->realtime_msg.body.realtime->dataTPL.veh_auto_button=temp4[2];
    this->realtime_msg.body.realtime->dataTPL.trip_distance=(temp4[3] << 8 | temp4[4]);
    this->realtime_msg.body.realtime->dataTPL.trip_left_mileage=(temp4[5] << 8 |temp4[6]);
    for(int i=0; i<10; i++){
        this->realtime_msg.body.realtime->dataTPL.fusion_coordinate[i]=temp4[i+7];
    }
    this->realtime_msg.body.realtime->dataTPL.current_speed=(temp4[17] << 8 | temp4[18]);
    this->realtime_msg.body.realtime->dataTPL.trip_current_id[0]=temp4[19];
    this->realtime_msg.body.realtime->dataTPL.trip_current_id[1]=temp4[20];
    this->realtime_msg.body.realtime->dataTPL.trip_current_id[2]=temp4[21];
    this->realtime_msg.body.realtime->dataTPL.trip_current_id[3]=temp4[22];
    this->realtime_msg.body.realtime->dataTPL.trip_target_id[0]=temp4[23];
    this->realtime_msg.body.realtime->dataTPL.trip_target_id[1]=temp4[24];
    this->realtime_msg.body.realtime->dataTPL.trip_target_id[2]=temp4[25];
    this->realtime_msg.body.realtime->dataTPL.trip_target_id[3]=temp4[26];
    for(int i=0; i<8; i++){
        this->realtime_msg.body.realtime->dataTPL.RTC_time[i]=temp4[i+27];
    }

    this->realtime_msg.header.attribute=Tool::getTotalUnionSize(this->flag,this->realtime_msg.body);
    //this->realtime_msg.header.attribute=get_attribute(this->realtime_msg.body);
    this->realtime_msg.checking_code=0x00;


}
void Tcper::writeToBuffer(message msg){
   Tcper::buffer.push((Tool::transfer(msg,this->flag)));
   this->flag=1;
}
void Tcper::get_heartbeat_msg(){

    this->heart_msg.identification_1=0x7e;
    this->heart_msg.identification_2=0x7e;
    this->heart_msg.header.id=0x0FFF;
    this->heart_msg.header.serial_numble=Tool::generateRandomUInt16();
    this->heart_msg.header.attribute=0x0000;
    this->heart_msg.checking_code=0x00;

    std::vector<uint8_t> bytes;
    bytes.push_back(this->heart_msg.identification_1);
    //uint16_t network_id = htons(data.header.id);
    bytes.push_back((this->heart_msg.header.id) & 0xFF);
    bytes.push_back((this->heart_msg.header.id >> 8) & 0xFF);
    bytes.push_back((this->heart_msg.header.attribute) & 0xFF);
    bytes.push_back((this->heart_msg.header.attribute >> 8) & 0xFF);
    bytes.push_back((this->heart_msg.header.serial_numble) & 0xFF);
    bytes.push_back((this->heart_msg.header.serial_numble >> 8) & 0xFF);
    bytes.push_back(this->heart_msg.checking_code);
    bytes.push_back(this->heart_msg.identification_2);
    std::vector<uint8_t> bytes_;
    uint8_t checksum = 0x00;
    for (size_t i = 0; i < bytes.size(); i++) {
        if(i>0 && i<bytes.size()-2 ){
            checksum ^= bytes[i];
            bytes_.push_back(bytes[i]);
        }
        else if(i==bytes.size()-2 ){
           bytes_.push_back(checksum);
        }
        else{
           bytes_.push_back(bytes[i]);
        }

    }

    this->heart_vec.push_back(bytes_[0]);
    for (uint i=1;i<bytes_.size()-1;i++)  {
        if (bytes_[i] == 0x7e) {
            this->heart_vec.push_back(0x7d);
            this->heart_vec.push_back(0x02);
        } else if (bytes_[i] == 0x7d ) {
            this->heart_vec.push_back(0x7d);
            this->heart_vec.push_back(0x01);
        } else {
            this->heart_vec.push_back(bytes_[i]);
        }
    }
    this->heart_vec.push_back(bytes_[bytes_.size()-1]);
     bytes.clear();
     bytes_.clear();


}


void Tcper::keep_alive(){

std::thread heartbeat_thread([=](){
    while (true) {
    if(this->net_flag==0 && getsockopt(this->sockfd, SOL_SOCKET, SO_ERROR, &error, &len)==0){
       // 发送数据
        get_heartbeat_msg();
        ssize_t bytesSent = send(this->sockfd, this->heart_vec.data(), this->heart_vec.size(), 0);
       if (bytesSent == -1) {
             std::cerr << "Failed to send heartbeat_msg." << std::endl;
             break;
       }

       std::cout << "Sent heartbeat_msg： " << bytesSent << " bytes of data." << std::endl;
       this->heart_vec.clear();
       // 暂停35秒
       std::this_thread::sleep_for(std::chrono::seconds(35));
       //std::cout<<"这是心跳报文"<<std::endl;
     }
    }

 });
heartbeat_thread.detach();

}
void Tcper::recieve(uint8_t (&buffer)[1024],ssize_t bytesRead){
    if(bytesRead<20){
        recieve_realtime_message msg=Tool::reverse_transfer_realtime(buffer,bytesRead);
        //对于应答id为：0x0C00的不作处理(心跳回应），若应答id为：0x0C02，进行实时上传结果处理
        if(msg.header.id==0x0C02){
          std::cout<<"实时回应结果："<<std::dec<<static_cast<int>(msg.result)<<std::endl;        

        }
        if(msg.header.id==0x0C00){
          std::cout<<"心跳回应结果："<<std::dec<<static_cast<int>(msg.result)<<std::endl;
        }
    }
    else{
        recieve_authentication msg=Tool::reverse_transfer_authentication(buffer,bytesRead);
        if(msg.header.id==0x0C01){
        std::cout<<"收到鉴权回应，鉴权结果："<<std::dec<<static_cast<int>(msg.result)<<std::endl;
        }
    }

}


void Tcper::inspect_buffer(){

    std::thread inspect_buffer_thread([=](){
    while (1) {
    if(buffer_queue.empty()!=1){
       this->get_realtime_message(buffer_queue.front());
       this->writeToBuffer(this->realtime_msg);
       buffer_queue.pop();
     }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    });
    inspect_buffer_thread.detach();

}

//检查连接状态
void Tcper::inspect_connect(){
    std::thread reconnect_thread([=]{
    while(true){
    sleep(5);
    std::string host = "www.baidu.com";
    std::string command = "ping -c 1 " + host+" 2>&1 > /dev/null";
    this->net_flag = system(command.c_str());
    if (this->net_flag != 0) {

            std::cerr << "Tcp_connection is lost" << std::endl;
            reconnect_tcp(this->sockfd);
            continue;


     }
    }
    });
    reconnect_thread.detach();
}

/*
void Tcper::inspect_connect(){
    std::thread reconnect_thread([=]{
    while(true){
    sleep(5);
    fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(this->sockfd, &readfds);
        struct timeval timeout;
        timeout.tv_sec = 10; // 设置超时时间为20秒
        timeout.tv_usec = 0;

        int ready = select(this->sockfd + 1, &readfds, NULL, NULL, &timeout);

        if (ready == -1) {
            perror("Error in select");
            break;
        } else if (ready == 0) {
            std::cout << "TCP connection is dead (timeout)" << std::endl;
            // 关闭连接并重新连接
            reconnect_tcp(this->sockfd);
        } else {

        }

     }


    });
    reconnect_thread.detach();
}
*/
