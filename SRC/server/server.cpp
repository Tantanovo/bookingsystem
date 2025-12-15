#include"server.hpp"


int main(){
    ser_listen server;
    if(!server.init_server()){
        perror("init server err");
        return -1;
    }
    struct event_base* base=event_base_new();
    if(!base){
        perror("event base err");
        return -1;
    }
    server.set_base(base);
    struct event*ev=event_new(base,server.getsockfd(),EV_READ|EV_PERSIST,ser_listen::accpet_listen,&server);
    if(!ev){
        perror("event new err");
        return -1;
    }
    //添加事件
    event_add(ev,NULL);
    //进入事件循环
    event_base_dispatch(base);
    event_free(ev);//事件
    event_base_free(base);//base
    
}