#include"server.hpp"


int main(){
    listen_server server;
    if(!server.init_server()){
        perror("init server err");
        return -1;
    }
    //设置libevent base
    struct event_base* base=event_base_new();
    if(!base){
        perror("event base err");
        return -1;
    }
    //设置listen_server中的libevent中的base   当程序处理客户端连接、创建新事件（如客户端读写事件）、停止事件循环时，还需要用到这个 base 指针；
    server.set_base(base);
    //创建事件
    struct event* ev=event_new(base,server.getser_sockfd(),EV_READ|EV_PERSIST,listen_server::client_accept,&server);
    if(!ev){
        perror("event new err");
        return -1;
    }
    //添加事件
    event_add(ev,NULL);
    //进入事件循环
    event_base_dispatch(base);
    //释放事件
    event_free(ev);
    //释放base
    event_base_free(base);
}