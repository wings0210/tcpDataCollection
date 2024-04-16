#include"HTTP/httper.h"


Httper::Httper(){
    // 初始化CURL库
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // 创建CURL句柄
    this->curl = curl_easy_init();
}
Httper::~Httper(){

}
