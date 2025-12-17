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
    cout<<"输入手机号码"<<endl;
    cin>>usertel;
    cout<<"输入用户名"<<endl;
    cin>>username;string passwd,tmp;
    cout<<"输入密码"<<endl;
    cin>>passwd;
    cout<<"请再输入一次密码,确保一致:"<<endl;
    cin>>tmp;
    if(usertel.empty()||username.empty()){
        cout<<"手机或用户名不能为空"<<endl;
        return;
    }
    if(passwd.compare(tmp)!=0){
        cout<<"密码不一致"<<endl;
        return;
    }
    Json::Value val;
    val["type"]=ZC;
    val["user_tel"]=usertel;
    val["user_name"]=username;
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
        cout<<"注册失败"<<endl;
        return;
    }

    flag=true;
    cout<<"注册成功"<<endl;
    return;
}
void connect_client::user_show_ticket(){
    Json::Value val;
    val["type"]=CKYY;
    send(cli_sockfd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
    char buff[4096]={0};
    int n=recv(cli_sockfd,buff,4095,0);
    if(n<=0){
        cout<<"ser close"<<endl;
        return;
    }
 
    vval.clear();
    Json::Reader Read;
    if(!Read.parse(buff,vval)){
        cout<<"json解析失败"<<endl;
        return;
    }
 
    string st=vval["status"].asString();
    if(st.compare("OK")!=0){
        cout<<"查询预约信息失败"<<endl;
        return;
    }
 
    int num=vval["num"].asInt();
    if(num==0){
        cout<<"没有可预约的信息"<<endl;
        return;
    }
 
    cout<<"编号  地点名称  总票数 已预订 时间"<<endl; 
    
    for(int i=0;i<num;i++){
        cout<<"-----------------------------------------------"<<endl;
        cout<<"|"<<vval["arr"][i]["tk_id"].asString()<<"    ";
        cout<<vval["arr"][i]["add"].asString()<<"    ";
        cout<<vval["arr"][i]["max"].asString()<<"    ";
        cout<<vval["arr"][i]["num"].asString()<<"    ";
        cout<<vval["arr"][i]["use_date"].asString()<<"|"<<endl;
        cout<<"-----------------------------------------------"<<endl;
    }
    cout<<endl;
}
void connect_client::user_subscribe_ticket(){
    user_show_ticket();
    cout<<"请输入要预订的编号："<<endl;
    int index=0;
    cin>>index;
    //index  有效性检查
    Json::Value val;
    val["type"]=YD;
    val["user_tel"]=usertel;
    val["index"]=index;
    send(cli_sockfd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
    char buff[256]={0};
    int n=recv(cli_sockfd,buff,255,0);
    if(n<=0){
        cout<<"ser close"<<endl;
        return;
    }
    val.clear();
    Json::Reader Read;
    if(!Read.parse(buff,val)){
        cout<<"json解析失败"<<endl;
        return;
    }
 
    string st=val["status"].asString();
    if(st.compare("OK")!=0){
        cout<<"预定失败"<<endl;
        return;
    }
    cout<<"预定成功"<<endl;
    cout<<endl;
}
void connect_client::user_show_sub_ticket(){
    Json::Value val;
    val["type"]=CKYD;
    val["user_tel"]=usertel;
    send(cli_sockfd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
    char buff[4096]={0};
    int n=recv(cli_sockfd,buff,4095,0);
    if(n<=0){
        cout<<"ser close"<<endl;
        return;
    }
 
    vval.clear();
    Json::Reader Read;
    if(!Read.parse(buff,vval)){
        cout<<"json解析失败"<<endl;
        return;
    }
 
    string st=vval["status"].asString();
    if(st.compare("OK")!=0){
        cout<<"查询预约信息失败"<<endl;
        return;
    }
 
    int num=vval["num"].asInt();
    if(num==0){
        cout<<"没有预约信息"<<endl;
        return;
    }
    cout << "我的预约记录：" << endl;
    cout << "预约ID  项目编号  地点      可使用日期  预约时间" << endl;
    for (int i = 0; i < num; i++) {
        cout << "-------------------------------------------------------------------" << endl;
        cout << "|" << vval["arr"][i]["yd_id"].asString() << "       ";    // 预约记录ID（sub_ticket.yd_id）
        cout << vval["arr"][i]["tk_id"].asString() << "    ";         // 项目编号（ticket_info.tk_id）
        cout << vval["arr"][i]["addr"].asString() << "    ";          // 预约地点（ticket_info.addr）
        cout << vval["arr"][i]["use_date"].asString() << "    ";      // 可使用日期（ticket_info.use_date）
        cout << vval["arr"][i]["curr_time"].asString() << "|" << endl;// 预约时间（sub_ticket.curr_time）
        cout << "-------------------------------------------------------------------" << endl;
    }
    cout << endl;
}
void connect_client::user_cancel_sub_ticket(){
    user_show_sub_ticket();
    val["type"]=QXYD;
    val["user_name"]=username;
    val["user_tel"]=usertel;
    int yd_id;
    cout<<"请输入一个要取消的预约id"<<endl;
    cin>>yd_id;
    val["yd_id"]=yd_id;
    send(cli_sockfd,val.toStyledString().c_str(),strlen(val.toStyledString().c_str()),0);
    char buff[256]={0};
    int n=recv(cli_sockfd,buff,255,0);
    if(n<=0){
        cout<<"ser close"<<endl;
        return;
    }
    val.clear();
    Json::Reader read;//JSON 文本 → 结构化数据” 的转换
    if(!read.parse(buff,val)){
        cout<<"json解析失败"<<endl;
        return;
    }
    string st=vval["status"].asString();
    if(st.compare("OK")!=0){
        cout << "输入错误:该预约ID不存在或您无权取消" << endl;
    }
    cout<<"预约取消成功!"<<endl;
    return ;
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