#include"server.hpp"

bool ser_listen::init_server(){
    ser_sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(ser_sockfd==-1){
        perror("socket err");
        return false;
    }
    
    // 设置SO_REUSEADDR选项，允许端口重用
    int opt = 1;
    if (setsockopt(ser_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt err");
        return false;
    }
    
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_port=htons(ser_port);
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=inet_addr(ser_ip.c_str());
    int res=bind(ser_sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res<0){
        perror("bind err");
        return false;
    }
    res=listen(ser_sockfd,5);
    if(res<0){
        perror("perror err");
        return false;
    }
    cout<<"listen success!"<<endl;
    return true; 
}

int ser_listen::accpet_listen(){
    int c=accept(ser_sockfd,NULL,NULL);
    if(c<0){
        perror("accept err\n");
        return -1;
    }
    return c;
}
