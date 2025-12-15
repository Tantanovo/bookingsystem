#ifndef SERVER_HPP
#define SERVER_HPP

#include<iostream>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<event2/event.h>
using namespace std;
class ser_listen{
private:
    int ser_sockfd;
    int ser_port;
    string ser_ip;
    struct event_base* ser_base;
public:
    ser_listen(){
        ser_sockfd=-1;
        ser_port=6000;
        ser_ip="127.0.0.1";
    }
    ser_listen(int port,string ip):ser_port(port),ser_ip(ip){
        ser_sockfd=-1;
    }
    bool init_server();
    static void accpet_listen(int fd, short events, void *arg);
    int getsockfd(){
        return ser_sockfd;
    }
    void set_base(struct event_base* base){
        this->ser_base=base;
    }



};




#endif
