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
#include<jsoncpp/json/json.h>
#include<mysql/mysql.h>
using namespace std;
enum op_type{
            DL=1,//登录
            ZC,//注册
            CKYY,//查看预约
            YD,//预定
            CKYD,//查看预定
            QXYD,//取消预订
            TC//退出
};
class ser_mysql{
private:
    MYSQL mysql_con;//连接句柄
    string db_ips;//主机地址/ip
    string db_username;
    string db_dbname;
    string db_passwd;
    // 事务开始（用于需要原子性的操作，如预订时减库存+记录预订）
    bool mysql_user_begin();
    bool mysql_user_commit();
    bool mysql_user_rollback();
    
public:
    ser_mysql(){
        db_ips="127.0.0.1";
        db_username="root";
        db_dbname="project_db";
        db_passwd="770202";
    }
    ~ser_mysql(){
        mysql_close(&mysql_con);
    }
 
    bool mysql_connectserver();
    bool mysql_register(const string &tel,const string &passwd,const string &name);
    bool mysql_login(const string&tel,const string &passwd,string &name);
    // 查看可预约信息：从数据库查询预约数据，存入JSON对象
    bool mysql_show_ticket(Json::Value&resval);
    bool mysql_subscribe_ticket(int tk_id,string tel);
    bool mysql_cancel_sub_ticket(int yd_id, const string& tel);
    bool mysql_show_sub_ticket(const string& tel, Json::Value& resval);
};
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
    int accpet_listen();
    int getsockfd(){
        return ser_sockfd;
    }
    void set_base(struct event_base* base){
        this->ser_base=base;
    }
    struct event_base* get_base()const{
        return ser_base;
    }

};

class server_con{
private:
    int c;//sockfd
    struct event* c_ev;
    Json::Value val;
public:
    server_con(int fd):c(fd){
        c_ev=NULL;
    }
    void set_ev(struct event*ev){
        c_ev=ev;
    }
    ~server_con(){
        event_free(c_ev);
        close(c);
    }
    void recv_data();
    void send_err();
    void send_ok();
    void user_register();//注册
    void user_login();//登录
    void user_show_ticket();
    void user_subscribe_ticket();
    void user_cancel_sub_ticket();
    void user_show_sub_ticket();
};




#endif
