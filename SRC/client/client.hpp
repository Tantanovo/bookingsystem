#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include<stdlib.h>
#include <string>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
//客户端连接部分
class connect_client
{
public:
    connect_client(){
        cli_sockfd=-1;
        cli_port=6000;
        cli_ip="127.0.0.1";
    }
    connect_client(int port,std::string ip):cli_port(port),cli_ip(ip){
        cli_sockfd=-1;
    };
    bool init_client();
    int getcli_sockfd(){
        return cli_sockfd;
    }
    void set_base(struct event_base* base){
        this->base=base;
    }
    struct event_base* get_base(){
        return base;
    }
private:
    int cli_sockfd;
    int cli_port;
    std::string cli_ip;
    struct event_base* base;
};
#endif