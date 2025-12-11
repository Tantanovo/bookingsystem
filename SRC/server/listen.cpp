#include "server.hpp"
bool listen_server:: init_server(){
    ser_sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(ser_sockfd<0){
        perror("socket err");
        return false;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(ser_ip.c_str());
    addr.sin_port=htons(ser_port);
    int res=bind(ser_sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(res<0){
        perror("bind err");
        return false;
    }
    res=listen(ser_sockfd,5);
    if(res<0){
        perror("listen err");
        return false;
    }
    std::cout<<"bind success!"<<std::endl;
    std::cout<<"listen port:"<<ser_port<<std::endl;
    return true;
}

void listen_server::client_accept(evutil_socket_t fd, short events, void *arg){
    listen_server* server = static_cast<listen_server*>(arg);
    int client_sockfd=accept(server->getser_sockfd(),NULL,NULL);
    if(client_sockfd<0){
        perror("accept err");
        return;
    }
    std::cout<<"accept client:"<<client_sockfd<<std::endl;
}