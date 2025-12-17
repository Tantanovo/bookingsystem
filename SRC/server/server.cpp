#include"server.hpp"
void server_con::send_err(){
    Json::Value res_val;
    res_val["status"]="ERR";
    send(c,res_val.toStyledString().c_str(),strlen(res_val.toStyledString().c_str()),0);
}
 
void server_con::send_ok(){
    Json::Value res_val;
    res_val["status"]="OK";
    send(c,res_val.toStyledString().c_str(),strlen(res_val.toStyledString().c_str()),0);
}

void server_con::user_register(){
    string username,usertel,userpasswd;
    usertel=val["user_val"].asString();
    username=val["user_name"].asString();
    userpasswd=val["user_passwd"].asString();
    if(usertel.empty()||username.empty()||userpasswd.empty()){
        send_err();
        return ;
    }
    ser_mysql ser;
    if(!ser.mysql_connectserver()){
        send_err();
        return;
    }
 
    if(!ser.mysql_register(usertel,userpasswd,username)){
        send_err();
        return;
    }
    send_ok();
    return;
}
void server_con::user_login(){
    string usertel,userpasswd,username;
    usertel=val["user_val"].asString();
    userpasswd=val["user_passwd"].asString();
    ser_mysql ser;
    if(!ser.mysql_connectserver()){
        send_err();
        return;
    }
    if(!ser.mysql_login(usertel,userpasswd,username)){
        send_err();
        return;
    }
 
    Json::Value res_val;
    res_val["status"]="OK";
    res_val["user_name"]=username;
    send(c,res_val.toStyledString().c_str(),strlen(res_val.toStyledString().c_str()),0);
    return;
}
void server_con::user_show_ticket(){
    Json::Value resval;
    ser_mysql ser;
    if(!ser.mysql_connectserver()){
        send_err();
        return;
    }
    if(!ser.mysql_show_ticket(resval)){
        send_err();
        return;
    }
    send(c,resval.toStyledString().c_str(),strlen(resval.toStyledString().c_str()),0);
    return;
};
void server_con::user_subscribe_ticket(){
    int tk_id=val["index"].asInt();
    string tel=val["tel"].asString();
    ser_mysql ser;
    if(!ser.mysql_connectserver()){
        cout<<"connect mysql err"<<endl;
        send_err();
        return;
    }
    if(!ser.mysql_subscribe_ticket(tk_id,tel)){
        send_err();
        return;
    }
    send_ok();
    return;
};
void server_con::user_cancel_sub_ticket(){
    int tk_id=val["index"].asInt();
    string tel=val["tel"].asString();
    Json::Value resval;
    ser_mysql ser;
    if(!ser.mysql_connectserver()){
        cout<<"connect mysql err"<<endl;
        send_err();
        return;
    }
    if(!ser.mysql_cancel_sub_ticket(tk_id,tel)){
        send_err();
        return;
    }
    resval["status"] = "OK";  
    send(c, resval.toStyledString().c_str(), strlen(resval.toStyledString().c_str()), 0);

};
void server_con::user_show_sub_ticket(){
    string tel=val["tel"].asString();
    ser_mysql ser;
    if(!ser.mysql_connectserver()){
        cout<<"connect mysql err"<<endl;
        send_err();
        return;
    }
    Json::Value resval;
    if(!ser.mysql_show_sub_ticket(tel,resval)){
        send_err();
        return;
    }
    send(c, resval.toStyledString().c_str(), strlen(resval.toStyledString().c_str()), 0);
};
void server_con::recv_data(){
    char buff[256]={0};
    int n=recv(c,buff,255,0);
    if(n<=0){
        cout<<"client close"<<endl;
        delete this;
        return;
    }
    cout<<"recv:"<<buff<<endl;
 
    Json::Reader Read;
    if(!Read.parse(buff,val)){
        cout<<"recv_data:解析json失败"<<endl;
        send_err();
        return;
    }
    int ops=val["type"].asInt();
    switch(ops){
                case DL://登陆
                user_login();
                break;
                case ZC://注册
                user_register();
                break;
                case CKYY://查看预约
                user_show_ticket();
                break;
                case YD://预订
                user_subscribe_ticket();
                break;
                case QXYD://取消预订
                user_cancel_sub_ticket();
                break;
                case CKYD://查看预订
                user_show_sub_ticket();
                break;
                case TC://退出
                break;
            default:
            cout<<"输入无效"<<endl;
            break;
    }
}

void SOCK_CON_CALLBACK(int sockfd,short ev,void *arg){
    server_con *q=(server_con*)arg;
    if(q==nullptr)return;
    if(ev&EV_READ){
        q->recv_data();
    }
}
void SOCK_LIS_CALLBACK(int sockfd,short ev,void *arg){
    ser_listen *p=(ser_listen*)arg;//arg收数据
    if(p==nullptr)return;
    if(ev&EV_READ){
        int c=p->accpet_listen();
        if(c==-1)return;
        cout<<"accept c="<<c<<endl;
        server_con *q=new server_con(c);
        struct event*c_ev=event_new(p->get_base(),c,EV_READ|EV_PERSIST,SOCK_CON_CALLBACK,q);
        if(c_ev==NULL){
            close(c);
            delete q;
            return;
        }
        q->set_ev(c_ev);
        //添加到libevent
        event_add(c_ev,NULL);
    }
}
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
    struct event*ev=event_new(base,server.getsockfd(),EV_READ|EV_PERSIST,SOCK_LIS_CALLBACK,&server);
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