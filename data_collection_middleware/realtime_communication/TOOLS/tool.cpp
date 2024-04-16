#include"TOOLS/tool.h"

//转换为字节流与重新生成校验码
std::vector<uint8_t> Tool::structToBytes(message data,int flag) {
    /*
    std::vector<uint8_t> bytes;
    struct a{
        std::string c;
        uint8_t b;
    };
    struct a a1;
    a1.c="hello";
    a1.b=0x01;

    size_t strLength = a1.c.size();
    //bytes.push_back(static_cast<uint8_t>(strLength));

    // 将字符串内容添加到字节序列
    for (size_t i = 0; i < strLength; i++) {
        bytes.push_back(static_cast<uint8_t>(a1.c[i]));
    }

    // 添加 uint8_t 类型的成员
    bytes.push_back(a1.b);

    return bytes;

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&a1);

    std::vector<uint8_t> bytes(ptr, ptr + sizeof(a1.c.size()));
    bytes.push_back(a1.b);
    return  bytes;

    */
    std::vector<uint8_t> bytes;
    //std::cout<<sizeof(data)<<std::endl;
    //data.body.realtime!=nullptr

    if(flag==1){
       bytes.push_back(data.identification_1);
       //uint16_t network_id = htons(data.header.id);
       bytes.push_back((data.header.id) & 0xFF);
       bytes.push_back((data.header.id >> 8) & 0xFF);
       bytes.push_back((data.header.attribute) & 0xFF);
       bytes.push_back((data.header.attribute >> 8) & 0xFF);
       bytes.push_back((data.header.serial_numble) & 0xFF);
       bytes.push_back((data.header.serial_numble >> 8) & 0xFF);
       for (size_t i = 0; i < data.body.authentication->tag.length(); i++) {
           bytes.push_back(static_cast<uint8_t>(data.body.authentication->tag[i]));
       }
       bytes.push_back(data.body.authentication->ctri_type);
       bytes.push_back(data.body.authentication->status);
       bytes.push_back(data.body.authentication->terminal_code_length);
       /*
       for (size_t i = 0; i < data.body.authentication->terminal_code.length(); i++) {
           bytes.push_back(static_cast<uint8_t>(data.body.authentication->terminal_code[i]));
       }
       */
       bytes.push_back(data.body.authentication->SIM_length);
       /*
       for (size_t i = 0; i < data.body.authentication->SIM.length(); i++) {
           bytes.push_back(static_cast<uint8_t>(data.body.authentication->SIM[i]));
       }
       */
       bytes.push_back(data.body.authentication->ICCID_length);
       /*
       for (size_t i = 0; i < data.body.authentication->ICCID.length(); i++) {
           bytes.push_back(static_cast<uint8_t>(data.body.authentication->ICCID[i]));
       }
       */
       bytes.push_back(data.body.authentication->Vin_length);
       for (size_t i = 0; i < data.body.authentication->Vin.length(); i++) {
           bytes.push_back(static_cast<uint8_t>(data.body.authentication->Vin[i]));
       }
       bytes.push_back(data.checking_code);
       bytes.push_back(data.identification_2);

       delete data.body.authentication;

    }
    else if(flag==2){

        bytes.push_back(data.identification_1);
        //uint16_t network_id = htons(data.header.id);
        bytes.push_back((data.header.id) & 0xFF);
        bytes.push_back((data.header.id >> 8) & 0xFF);
        bytes.push_back((data.header.attribute) & 0xFF);
        bytes.push_back((data.header.attribute >> 8) & 0xFF);
        bytes.push_back((data.header.serial_numble) & 0xFF);
        bytes.push_back((data.header.serial_numble >> 8) & 0xFF);

        for(int i=0; i<8;  i++){
        bytes.push_back(data.body.realtime->current_time[i]);
        }

        bytes.push_back(data.body.realtime->RTC_tag);
        for(int i=0; i<8;   i++){
        bytes.push_back(data.body.realtime->RTC_time[i]);
        }

        bytes.push_back(data.body.realtime->location_tag);
        for(int i=0; i<21;   i++){
        bytes.push_back(data.body.realtime->location[i]);
        }

        bytes.push_back(data.body.realtime->selfdefined_tag);

        bytes.push_back(data.body.realtime->dataTPL.data_length_flag1);
        bytes.push_back(data.body.realtime->dataTPL.data_length_flag2);
        bytes.push_back(data.body.realtime->dataTPL.template_flag1);
        bytes.push_back(data.body.realtime->dataTPL.template_flag2);
        bytes.push_back(data.body.realtime->dataTPL.veh_status_low);
        bytes.push_back(data.body.realtime->dataTPL.veh_status_high);
        bytes.push_back(data.body.realtime->dataTPL.veh_auto_button);

        bytes.push_back((data.body.realtime->dataTPL.trip_distance)& 0xFF);
        bytes.push_back((data.body.realtime->dataTPL.trip_distance >> 8)& 0xFF);

        bytes.push_back((data.body.realtime->dataTPL.trip_left_mileage)& 0xFF);
        bytes.push_back((data.body.realtime->dataTPL.trip_left_mileage >> 8)& 0xFF);
        for(int i=0;i<10;i++){
           bytes.push_back(data.body.realtime->dataTPL.fusion_coordinate[i]);

        }

        bytes.push_back((data.body.realtime->dataTPL.current_speed)& 0xFF);
        bytes.push_back((data.body.realtime->dataTPL.current_speed>> 8)& 0xFF);

        for(int i=0; i<4; i++){
            bytes.push_back(data.body.realtime->dataTPL.trip_current_id[i]);
        }
        for(int i=0; i<4; i++){
            bytes.push_back(data.body.realtime->dataTPL.trip_target_id[i]);
        }
        for(int i=0;i<8;i++){
           bytes.push_back(data.body.realtime->dataTPL.RTC_time[i]);

        }


        bytes.push_back(data.checking_code);
        bytes.push_back(data.identification_2);

        delete data.body.realtime;

    }


    else{

    }

//  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);

    std::vector<uint8_t> new_bytes;
    uint8_t checksum = 0x00;
    for (size_t i = 0; i < bytes.size(); i++) {
        if(i>0 && i<bytes.size()-2){
            checksum ^= bytes[i];
            new_bytes.push_back(bytes[i]);
        }
        else if(i==bytes.size()-2){
           new_bytes.push_back(checksum);
        }
        else{

           new_bytes.push_back(bytes[i]);
        }
    }

    return new_bytes;

}

//转义
std::vector<uint8_t> Tool::transfer(message msg,int flag) {

    std::vector<uint8_t> bytes = structToBytes(msg,flag);
    std::vector<uint8_t> escapedData;
    escapedData.push_back(bytes[0]);
    for (uint i=1;i<bytes.size()-1;i++)  {
        if (bytes[i] == 0x7e) {
            escapedData.push_back(0x7d);
            escapedData.push_back(0x02);
        } else if (bytes[i] == 0x7d ) {
            escapedData.push_back(0x7d);
            escapedData.push_back(0x01);
        } else {
            escapedData.push_back(bytes[i]);
        }
    }
    escapedData.push_back(bytes[bytes.size()-1]);
    return escapedData;
}

//鉴权回应反转义与反序列化
recieve_authentication Tool::reverse_transfer_authentication(uint8_t (&buffer)[1024],ssize_t bytesRead ){
    std::vector<uint8_t> bytes;
    bytes.push_back(buffer[0]);
    for (uint i = 1; i <bytesRead-1; i++) {
        if (buffer[i] ==0x7d ) {
            if (buffer[i + 1] == 0x02) {
                bytes.push_back(0x7e);
            } else if (buffer[i + 1] == 0x01) {
                bytes.push_back(0x7d);
            }
            i++; // 跳过转义序列的第二个字节
        } else {
            bytes.push_back(buffer[i]);
        }
    }
    bytes.push_back(buffer[bytesRead-1]);
    recieve_authentication msg;
    //std::cout<<sizeof(recieve_authentication)<<std::endl;
    if(bytes.size() == sizeof(recieve_authentication)){
         memcpy(&msg, bytes.data(), sizeof(recieve_authentication));
    }



    return msg;

}

//实时回应反转义与反序列化
recieve_realtime_message Tool::reverse_transfer_realtime(uint8_t (&buffer)[1024],ssize_t bytesRead){
    std::vector<uint8_t> bytes;
    bytes.push_back(buffer[0]);
    for (uint i = 1; i <bytesRead-1; i++) {
        if (buffer[i] ==0x7d ) {
            if (buffer[i + 1] == 0x02) {
                bytes.push_back(0x7e);
            } else if (buffer[i + 1] == 0x01) {
                bytes.push_back(0x7d);
            }
            i++; // 跳过转义序列的第二个字节
        } else {
            bytes.push_back(buffer[i]);
        }
    }
    bytes.push_back(buffer[bytesRead-1]);
    recieve_realtime_message msg;
    //std::cout<<sizeof(recieve_realtime_message)<<std::endl;
    if(bytes.size() == sizeof(recieve_realtime_message)){
         memcpy(&msg, bytes.data(), sizeof(recieve_realtime_message));
    }

    return msg;

}
uint16_t Tool:: generateRandomUInt16() {
    // 创建随机数引擎
    std::random_device rd;
    std::mt19937 gen(rd());

    // 创建分布，限定随机数范围为 [0, 65535]，即 uint16_t 的取值范围
    std::uniform_int_distribution<uint16_t> dis(0, std::numeric_limits<uint16_t>::max());

    // 生成随机数并返回
    return dis(gen);
}






//暂时不用
void Tool::timestampToByteArray(uint64_t timestamp, uint8_t byteArray[8]) {
    // 按照指定格式提取时间戳的各个部分
    uint16_t year = (timestamp / 10000000000ULL) % 100; // 年十位和年个位
    uint8_t month = (timestamp / 100000000ULL) % 10;   // 月十位
    uint8_t day = (timestamp / 1000000ULL) % 10;       // 日十位
    uint8_t hour = (timestamp / 10000ULL) % 10;        // 时十位
    uint8_t minute = (timestamp / 100ULL) % 10;        // 分十位
    uint8_t second = (timestamp / 1ULL) % 10;          // 秒十位
    uint8_t millisecond = (timestamp / 10ULL) % 10;    // 毫秒千位和百位
    uint8_t millisecond2 = (timestamp / 100ULL) % 10;  // 毫秒十位和个位

    // 将提取的部分存储到数组中
    byteArray[0] = (year * 10) + month;
    byteArray[1] = (day * 10) + hour;
    byteArray[2] = (minute * 10) + second;
    byteArray[3] = (millisecond * 10) + millisecond2;
}

uint64_t Tool::get_timestamp(){
    auto now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration tp = now.time_since_epoch();
    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(tp);

    // 转换为 uint64_t 类型的时间戳
    uint64_t currentTimestamp = static_cast<uint64_t>(seconds.count());
    return currentTimestamp;
}

//1,当前补时间;2,实时时间
std::vector<uint8_t> Tool::get_time(int type){

        time_t currentTime;
        time(&currentTime);

        // 将时间戳拆分为年、月、日、时、分、秒和毫秒
        struct tm *timeinfo;
        timeinfo = localtime(&currentTime);

        int year = timeinfo->tm_year + 1900; // 年份需要加上1900
        int month = timeinfo->tm_mon + 1;    // 月份是从0开始的，所以要加1
        int day = timeinfo->tm_mday;
        int hour = timeinfo->tm_hour;
        int minute = timeinfo->tm_min;
        int second = timeinfo->tm_sec;

        // 获取毫秒
        struct timespec currentTimeSpec;
        clock_gettime(CLOCK_REALTIME, &currentTimeSpec);
        int millisecond = currentTimeSpec.tv_nsec / 1000000; // 将纳秒转换为毫秒

    if(type==1){
        // 将数字拆分为高4位和低4位
        uint8_t byteArray[8] = {
            static_cast<uint8_t>(((year-2000) / 10+8) << 4 | ((year-2000) % 10)),
            static_cast<uint8_t>((month / 10) << 4 | (month % 10)),
            static_cast<uint8_t>((day / 10) << 4 | (day % 10)),
            static_cast<uint8_t>((hour / 10) << 4 | (hour % 10)),
            static_cast<uint8_t>((minute / 10) << 4 | (minute % 10)),
            static_cast<uint8_t>((second / 10) << 4 | (second % 10)),
            static_cast<uint8_t>((millisecond / 100) << 4 | ((millisecond % 100) / 10)),
            static_cast<uint8_t>((millisecond % 10)),
        };
        std::vector<uint8_t> v;
        for(int i=0; i<8; i++){
            v.push_back(byteArray[i]);
        }

        return v;
    }
    else{
        uint8_t byteArray[8] = {
            static_cast<uint8_t>(((year-2000) / 10) << 4 | ((year-2000) % 10)),
            static_cast<uint8_t>((month / 10) << 4 | (month % 10)),
            static_cast<uint8_t>((day / 10) << 4 | (day % 10)),
            static_cast<uint8_t>((hour / 10) << 4 | (hour % 10)),
            static_cast<uint8_t>((minute / 10) << 4 | (minute % 10)),
            static_cast<uint8_t>((second / 10) << 4 | (second % 10)),
            static_cast<uint8_t>((millisecond / 100) << 4 | ((millisecond % 100) / 10)),
            static_cast<uint8_t>((millisecond % 10)),
        };
        /*
        for (int i = 0; i < 8; i++) {
            std::cout << std::hex << static_cast<int>(byteArray[i]) << " ";
        }
        */
        std::vector<uint8_t> v;
        for(int i=0; i<8; i++){
            v.push_back(byteArray[i]);
        }

        return v;
    }


}

//获取定位表
std::vector<uint8_t> Tool::get_location(locations loc){
        long longitude_integer=static_cast<long>(loc.longitude);
        long latitude_integer=static_cast<long>(loc.latitude);
        long height_integer=static_cast<long>(loc.height);
        long azi_integer=static_cast<long>(loc.azi);
        long speed_integer=static_cast<long>(loc.speed);

        // 提取经度小数部分的各位数字
        double fractionalPart_1 = std::abs(loc.longitude - std::floor(loc.longitude));
        std::vector<int> longitude;
        //std::cout << "小数部分的各位数字： ";
        for (int i=0;fractionalPart_1 > 0.0 && i < 7;i++) {
            fractionalPart_1 *= 10.0;
            int digit = static_cast<int>(std::floor(fractionalPart_1));
            longitude.push_back(digit);
            //std::cout << digit << std::endl;
            fractionalPart_1 -= digit;

        }

        // 提取纬度小数部分的各位数字
        double fractionalPart_2 = std::abs(loc.latitude - std::floor(loc.latitude));
        std::vector<int> latitude;
            //std::cout << "小数部分的各位数字： ";
            for (int i=0;fractionalPart_2 > 0.0 && i < 7;i++) {
                fractionalPart_2 *= 10.0;
                int digit = static_cast<int>(std::floor(fractionalPart_2));
                latitude.push_back(digit);
                //std::cout << digit << std::endl;
                fractionalPart_2 -= digit;

            }
        //方向
        double fractionalPart_3 = std::abs(loc.azi - std::floor(loc.azi));
        int digit_azi=fractionalPart_3*10;
        //速度
        double fractionalPart_4 = std::abs(loc.speed - std::floor(loc.speed));
        int digit_speed=fractionalPart_4*10;
        //累计里程
        double fractionalPart_5 = std::abs(loc.accumulative_mileage - std::floor(loc.accumulative_mileage));
        int digit_acc=fractionalPart_5*10;
        std::vector<int> acc;
            // 将数取整，以处理整数部分
        long long number= static_cast<long long>(loc.accumulative_mileage);
        long long divisor = 1000000; // 从百万位开始
            while (divisor > 0) {
                int digit = static_cast<int>(number / divisor);
                acc.push_back(digit);
                // 更新除数，移动到下一个位数
                number %= divisor;
                divisor /= 10;
            }



    uint8_t byteArray[21]={
        //经度
       static_cast<uint8_t>((0+longitude_integer/100) << 4 | ((longitude_integer-(longitude_integer /100)*100)/10)),
       static_cast<uint8_t>(longitude_integer % 10 << 4 | longitude[0]),
       static_cast<uint8_t>(longitude[1] << 4 | longitude[2]),
       static_cast<uint8_t>(longitude[3] << 4 | longitude[4]),
       static_cast<uint8_t>(longitude[5] << 4 | longitude[6]),
       //纬度
       static_cast<uint8_t>((4+(latitude_integer/100)) << 4 | ((latitude_integer-(latitude_integer /100)*100)/10)),
       static_cast<uint8_t>(latitude_integer % 10 << 4 | latitude[0]),
       static_cast<uint8_t>(latitude[1] << 4 | latitude[2]),
       static_cast<uint8_t>(latitude[3] << 4 | latitude[4]),
       static_cast<uint8_t>(latitude[5] << 4 | latitude[6]),
       //高度
       static_cast<uint8_t>((0+height_integer/1000) << 4 | ((height_integer-(height_integer /1000)*1000)/100)),
       static_cast<uint8_t>(((height_integer-(height_integer /1000)*1000-((height_integer-(height_integer /1000)*1000)/100)*100)/10) << 4 | height_integer % 10),

        //方向

        static_cast<uint8_t>(azi_integer/100 << 4 | (azi_integer-100*(azi_integer/100))/10),
        static_cast<uint8_t>(azi_integer % 10 << 4 | digit_azi),
        //速度
        static_cast<uint8_t>(speed_integer/100 << 4 | (speed_integer-100*(speed_integer/100))/10 ),
        static_cast<uint8_t>(speed_integer % 10 << 4 | digit_speed),
        //累计里程

        static_cast<uint8_t>(acc[0] << 4 | acc[1] ),
        static_cast<uint8_t>(acc[2] << 4 | acc[3]),
        static_cast<uint8_t>(acc[4] << 4 | acc[5]),
        static_cast<uint8_t>(acc[6] << 4 | digit_acc),

    };
    std::vector<uint8_t> v;
    for(int i=0; i<21; i++){
        v.push_back(byteArray[i]);
    }

    return v;


}

//获取dataTPL
std::vector<uint8_t> Tool::get_self_defining_data(locations loc){


    long longitude_integer=static_cast<long>(loc.longitude);
    long latitude_integer=static_cast<long>(loc.latitude);

    uint16_t distance=loc.trip_distance;
    uint16_t left_distance=loc.trip_left_mileage;
    //速度
    uint16_t speed = round(loc.speed * 10);
    // 提取经度小数部分的各位数字
    double fractionalPart_1 = std::abs(loc.longitude - std::floor(loc.longitude));
    std::vector<int> longitude;
    //std::cout << "小数部分的各位数字： ";
    for (int i=0;fractionalPart_1 > 0.0 && i < 7;i++) {
        fractionalPart_1 *= 10.0;
        int digit = static_cast<int>(std::floor(fractionalPart_1));
        longitude.push_back(digit);
        //std::cout << digit << std::endl;
        fractionalPart_1 -= digit;

    }

    // 提取纬度小数部分的各位数字
    double fractionalPart_2 = std::abs(loc.latitude - std::floor(loc.latitude));
    std::vector<int> latitude;
        //std::cout << "小数部分的各位数字： ";
        for (int i=0;fractionalPart_2 > 0.0 && i < 7;i++) {
            fractionalPart_2 *= 10.0;
            int digit = static_cast<int>(std::floor(fractionalPart_2));
            latitude.push_back(digit);
            //std::cout << digit << std::endl;
            fractionalPart_2 -= digit;

        }




      uint8_t button_status;
      if(loc.veh_auto_button==0) button_status=0x00;
      else                       button_status=0x01;


      uint8_t low,high;
      if(loc.veh_status>=0 && loc.veh_status<=6){
          high=(0 << 4 | loc.veh_status);
          low=0x00;
      }
      else{
          high=0x05;
          low=0x01;
      }
      uint8_t byteArray[27]={
         //车辆状态码
         static_cast<uint8_t>(high),
         static_cast<uint8_t>(low),
          //自动按钮状态
         static_cast<uint8_t>(button_status),
         //单次行程总里程

          static_cast<uint8_t>((distance >> 8) & 0xFF),
          static_cast<uint8_t>(distance & 0xFF),

         //站点剩余里程
          static_cast<uint8_t>((left_distance >> 8) & 0xFF),
          static_cast<uint8_t>(left_distance & 0xFF),
          //经度
         static_cast<uint8_t>((0+longitude_integer/100) << 4 | ((longitude_integer-(longitude_integer /100)*100)/10)),
         static_cast<uint8_t>(longitude_integer % 10 << 4 | longitude[0]),
         static_cast<uint8_t>(longitude[1] << 4 | longitude[2]),
         static_cast<uint8_t>(longitude[3] << 4 | longitude[4]),
         static_cast<uint8_t>(longitude[5] << 4 | longitude[6]),
         //纬度
         static_cast<uint8_t>((4+(latitude_integer/100)) << 4 | ((latitude_integer-(latitude_integer /100)*100)/10)),
         static_cast<uint8_t>(latitude_integer % 10 << 4 | latitude[0]),
         static_cast<uint8_t>(latitude[1] << 4 | latitude[2]),
         static_cast<uint8_t>(latitude[3] << 4 | latitude[4]),
         static_cast<uint8_t>(latitude[5] << 4 | latitude[6]),
          //速度
          static_cast<uint8_t>((speed >> 8) & 0xFF),
          static_cast<uint8_t>(speed & 0xFF),
          //当前站点id
          static_cast<uint8_t>(loc.trip_current_id[0]),
          static_cast<uint8_t>(loc.trip_current_id[1]),
          static_cast<uint8_t>(loc.trip_current_id[2]),
          static_cast<uint8_t>(loc.trip_current_id[3]),
          //目标站点id
          static_cast<uint8_t>(loc.trip_target_id[0]),
          static_cast<uint8_t>(loc.trip_target_id[1]),
          static_cast<uint8_t>(loc.trip_target_id[2]),
          static_cast<uint8_t>(loc.trip_target_id[3])
      };
      std::vector<uint8_t> v;
      for(int i=0; i<27; i++){
          v.push_back(byteArray[i]);
      }

      std::vector<uint8_t> temp_v;
      temp_v=Tool::get_time(2);
      for(int i=0; i<8;  i++){
          v.push_back(temp_v[i]);
      }
      return v;

}

uint16_t Tool:: getTotalUnionSize(int flag,message_body& obj) {

    if (flag==1) {
        //uint16_t size=sizeof(obj.authentication->ctri_type)+sizeof(obj.authentication->ICCID_length)+sizeof(obj.authentication->SIM_length)+sizeof(obj.authentication->status)+sizeof(obj.authentication->tag)+sizeof(obj.authentication->terminal_code_length)+sizeof(obj.authentication->Vin_length)+sizeof(obj.authentication->Vin)+0x0000;
        return 0x0028;
    }
    else if(flag==2){
        size_t dynamicSize = sizeof(realtime_message);
        return dynamicSize+0x0000;
    }
    else{
        size_t dynamicSize = sizeof(responsing);
        return dynamicSize+0x0000;
    }

}





//折半插入排序
void Tool::binary_insert(int a[],int n){
    int i,j;
    for(i=1;   i<=n;  i++){
      if(a[i]<a[i-1]){
          int temp=a[i];
         int low=0;
         int high=i-1;
          while(low<=high){
                 int mid=(low+high)/2;
                 if(a[mid]>temp)  high=mid-1;
                 else  low=mid+1;
          }
          for(j=i-1; j>=high; j--){
            a[j+1]=a[j];
          }
          a[high+1]=temp;
      }
    }



}
//直接插入排序
void Tool::insert_select(int a[],int n){
   int i,j;
   for(i=1; i<n;  i++){
        if(a[i]<a[i-1]){
            int temp=a[i];
            for(j=i-1;  a[j]>temp;  j--){
               a[j+1]=a[j];
            }
            a[j+1]=temp;
        }
   }
}
