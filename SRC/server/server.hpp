#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event2/event.h>
#include <cstring>

//服务器连接部分    
class listen_server
{
public:
    listen_server(){
        ser_sockfd = -1;
        ser_port = 6000;
        ser_ip = "127.0.0.1";
    }
    listen_server(int port, std::string ip): ser_port(port), ser_ip(ip){
        ser_sockfd = -1;
    };
    bool init_server();
    static void client_accept(evutil_socket_t fd, short events, void *arg);
    int getser_sockfd(){
        return ser_sockfd;
    }
    void set_base(struct event_base* base){
        this->base = base;
    }
    struct event_base* get_base(){
        return base;
    }
private:
    int ser_sockfd;
    int ser_port;
    std::string ser_ip;
    struct event_base* base;
};
#endif // SERVER_H

