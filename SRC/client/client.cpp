#include"client.hpp"
void connect_client::print_info(){
     if(flag){
        cout<<"--已登陆-------用户名："<<username<<"-------"<<endl;
        cout<<"1:查看预约   2:预订  3: 查看我的预约  4:取消预约  5:退出"<<endl;
        cout<<"-------------------------------------------------"<<endl;
        cout<<"请输入选项编号："<<endl;
        cin>>user_op;
    }
    else{
        cout<<"---未登陆-----游客-----"<<endl;
        cout<<"1:登陆   2:注册  3: 退出 "<<endl;
        cout<<"-----------------"<<endl;
        cout<<"请输入选项编号："<<endl;
        cin>>user_op;
        if(user_op==3)user_op=TC;
    }
}
void connect_client::user_login(){
    string tel,passwd;
    cout<<"请输入手机号码"<<endl;
    cin>>tel;
    cout<<"请输入密码"<<endl;
    cin>>passwd;
    if(tel.empty()||passwd.empty()){
        cout<<"帐号或密码不能为空"<<endl;
        return;
    }
    Json::Value val;
    val["type"]=DL;
    val["user_tel"]=tel;
    val["user_passwd"]=passwd;
    send(cli_sockfd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
    
    char buff[256]={0};
    int n=recv(cli_sockfd,buff,255,0);
    if(n<=0){
        cout<<"ser close"<<endl;
        return;
    }
    val.clear();
    
    Json::Reader read;
    if(!read.parse(buff,val)){
        cout<<"解析json失败"<<endl;
        return;
    }

    string st=val["status"].asString();
    if(st.compare("OK")!=0){
        cout<<"登陆失败"<<endl;
        return;
    }

    flag=true;
    username=val["user_name"].asString();
    usertel=tel;
    cout<<"登陆成功"<<endl;
    return;

}
void connect_client::user_register(){
    

}
void connect_client::user_show_ticket(){

}
void connect_client::user_subscribe_ticket(){

}
void connect_client::user_show_sub_ticket(){

}
void connect_client::user_cancel_sub_ticket(){

}
void connect_client::run(){
    while(running){
        print_info();
        switch(user_op){
                case DL:
                user_login();
                break;
                case ZC:
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
                running=false;
                break;
            default:
            cout<<"输入无效"<<endl;
            break;
        }
    }
}

int main(){
    connect_client client;
    if(!client.init_client()){
        perror("init client err");
        return -1;
    }
    client.run();
    return 0;
}