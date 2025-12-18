#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include<stdlib.h>
#include <string>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include<event2/event.h>
#include<jsoncpp/json/json.h>
using namespace std;
const int offset=2;
enum op_type{
            DL=1,//登录
            ZC,//注册
            CKYY,//查看预约
            YD,//预定
            CKYD,//查看预定
            QXYD,//取消预订
            TC//退出
};

//客户端连接部分
class connect_client{
public:
    connect_client(){
        cli_sockfd=-1;
        cli_port=6000;
        cli_ip="127.0.0.1";
    }
    connect_client(int port,string ip):cli_port(port),cli_ip(ip){
        cli_sockfd=-1;
    };
    bool init_client();
    int getcli_sockfd(){
        return cli_sockfd;
    }
    void print_info();
    void user_register();//注册
    void user_login();//登录
    void user_show_ticket();
    void user_subscribe_ticket();
    void user_cancel_sub_ticket();
    void user_show_sub_ticket();
    void run();

private:
    int cli_sockfd;
    int cli_port;
    string cli_ip;
    bool flag;
    Json::Value val;
    int user_op;//选择
    string username;
    string usertel;
    bool running=true;
    Json::Value vval;
    
};


#endif