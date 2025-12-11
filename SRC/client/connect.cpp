#include"client.hpp"
bool connect_client:: init_client(){
    cli_sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(cli_sockfd<0){
        perror("socket err");
        return false;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(cli_ip.c_str());
    addr.sin_port=htons(cli_port);
    int res=connect(cli_sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(res<0){
        perror("connect err");
        return false;
    }
    std::cout<<"connect success!"<<std::endl;
    std::cout<<"connect server:"<<cli_sockfd<<std::endl;
    return true;
}
