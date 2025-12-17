#include"server.hpp"

bool ser_mysql:: mysql_connectserver(){
    MYSQL*mysql=mysql_init(&mysql_con);
    if(mysql==nullptr)return false;
    mysql_real_connect(mysql,db_ips.c_str(),db_username.c_str(),db_passwd.c_str(),db_dbname.c_str(),3306,NULL,0);
    if(mysql==NULL){
        cout<<"connect db server err"<<endl;
        return false;
    }
    return true;
};
bool ser_mysql::mysql_user_begin(){
    if(mysql_query(&mysql_con,"begin")!=0){
        return false;
    }
    return true;
}

bool ser_mysql::mysql_user_commit(){
    if(mysql_query(&mysql_con,"commit")!=0){
        return false;
    }
    return true;
}
 
bool ser_mysql::mysql_user_rollback(){
    if(mysql_query(&mysql_con,"rollback")!=0){
        return false;
    }
    return true;
}
/* user_info (用户
    ->     id INT PRIMARY KEY AUTO_INCREMENT,
    ->     tel VARCHAR(20) NOT NULL UNIQUE,
    ->     username VARCHAR(50) NOT NULL,
    ->     passwd VARCHAR(50) NOT NULL,
    ->     status INT DEFAULT 1
    -> );
 ticket_info (票
    ->     tk_id INT PRIMARY KEY AUTO_INCREMENT,
    ->     addr VARCHAR(100) NOT NULL,
    ->     max INT NOT NULL,
    ->     num INT DEFAULT 0,
    ->     use_date DATE NOT NULL
    -> );
 sub_ticket (预约记录
    ->     yd_id INT PRIMARY KEY AUTO_INCREMENT,
    ->     tk_id INT NOT NULL,
    ->     tel VARCHAR(20) NOT NULL,
    ->     curr_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    ->     FOREIGN KEY (tk_id) REFERENCES ticket_info(tk_id),
    ->     FOREIGN KEY (tel) REFERENCES user_info(tel)
    -> );
*/
bool ser_mysql::mysql_register(const string &tel,const string &passwd,const string &name){
    string sql=string("insert into  user_info values(0,'")+tel+string("','")+name+string("','")+passwd+string("',1)");
    if(mysql_query(&mysql_con,sql.c_str())!=0)return false;
    return true;
};

bool ser_mysql::mysql_login(const string&tel,const string &passwd,string &name){
    string sql=string("select username,passwd from user_info where tel=")+tel;
    if(mysql_query(&mysql_con,sql.c_str())!=0)return false;
    MYSQL_RES*r=mysql_store_result(&mysql_con);
    if(r==NULL)return false;
    int num=mysql_num_rows(r);//获取结果集有多少行，0行就是未查询到，意味着该用户没有注册
    if(num==0){
        mysql_free_result(r);
        return false;
    }
    MYSQL_ROW row=mysql_fetch_row(r);//获取一行

    name=row[0];
    string password=row[1];
    if(password.compare(passwd)!=0)return false;
    mysql_free_result(r);
    return true;
};
    // 查看可预约信息：从数据库查询预约数据，存入JSON对象
bool ser_mysql::mysql_show_ticket(Json::Value&resval){
    string sql="select tk_id,addr,max,num,use_date from ticket_info";
    if(mysql_query(&mysql_con,sql.c_str())!=0){
        cout<<"show ticket err"<<endl;
        return false;
    }
    MYSQL_RES*r=mysql_store_result(&mysql_con);
    if(r==nullptr)return false;
    int n=mysql_num_rows(r);
    if(n==0){
        resval["status"]="OK";
        resval["num"]=0;
        return true;
    }
 
    resval["status"]="OK";
    resval["num"]=n;
 
    for(int i=0;i<n;i++){
        MYSQL_ROW row=mysql_fetch_row(r);
        Json::Value tmp;
        tmp["tk_id"]=row[0];
        tmp["add"]=row[1];
        tmp["max"]=row[2];
        tmp["num"]=row[3];
        tmp["use_date"]=row[4];
        resval["arr"].append(tmp);
    }
    return true;
};
bool ser_mysql::mysql_subscribe_ticket(int tk_id,string tel){
    mysql_user_begin();
    //查询票
    string s1=string("select max,num from ticket_info where tk_id=")+to_string(tk_id);
    if(mysql_query(&mysql_con,s1.c_str())!=0){
        cout<<"查询max,num失败"<<endl;
        mysql_user_rollback();
        return false;
    }
    MYSQL_RES*r=mysql_store_result(&mysql_con);
    if(r==nullptr){
        cout<<"获取结果集失败"<<endl;
        mysql_user_rollback();
        return false;
    }
 
    int Num=mysql_num_rows(r);
    if(Num!=1){
        cout<<"记录行不为一"<<endl;
        mysql_user_rollback();
        return false;
    }
 
    MYSQL_ROW row=mysql_fetch_row(r);
    string str_max=row[0];//总票数
    string str_num=row[1];//当前已订票数
    int tk_max=atoi(str_max.c_str());
    int tk_num=atoi(str_num.c_str());
    if(tk_max<=tk_num){
        cout<<"没有可用的票"<<endl;
        mysql_user_rollback();
        return false;
    }
    tk_num++;
    //更新票
    string s2=("update ticket_info set num=")+ to_string(tk_num)+ string(" where tk_id=")+to_string(tk_id);
    if(mysql_query(&mysql_con,s2.c_str())!=0){
        cout<<"修改预订票数失败"<<endl;
        mysql_user_rollback();
        return false;
    }
 
    //插入已经预约的票
    string s3=string("insert into sub_ticket values(0,")+to_string(tk_id)+string(",'")+tel+string("',now())");
    if(mysql_query(&mysql_con,s3.c_str())!=0){
        cout<<"存入预订信息失败"<<endl;
        mysql_user_rollback();
        return false;
    }
 
 
    mysql_user_commit();
    return true;
};
bool ser_mysql::mysql_cancel_sub_ticket(int yd_id, const string& tel){
    mysql_user_begin();
    // 查询预约对应的tk_id（更新库存）
    string sql_query = "SELECT tk_id FROM sub_ticket WHERE yd_id = " + to_string(yd_id) + " AND tel = '" + tel + "'";
    if (mysql_query(&mysql_con, sql_query.c_str()) != 0) {
        cout << "查询tk_id失败:" << mysql_error(&mysql_con) << endl;
        mysql_user_rollback();
        return false;
    }
    MYSQL_RES* res = mysql_store_result(&mysql_con);
    if (res == NULL || mysql_num_rows(res) == 0) {
        cout << "未找到该预约记录(yd_id=" << yd_id << ",tel=" << tel << ")" << endl;
        mysql_free_result(res);
        mysql_user_rollback();
        return false;
    }
    MYSQL_ROW row = mysql_fetch_row(res);
    int tk_id = atoi(row[0]);  // 获取tk_id
    mysql_free_result(res);
 
    // 更新ticket库存 num-1
    string sql_update = "UPDATE ticket_info SET num = num - 1 WHERE tk_id = " + to_string(tk_id);
    if (mysql_query(&mysql_con, sql_update.c_str()) != 0) {
        cout << "更新库存失败：" << mysql_error(&mysql_con) << endl;
        mysql_user_rollback();
        return false;
    }
    // 删除sub_ticket中对应的预约记录
    string sql_delete = "DELETE FROM sub_ticket WHERE yd_id = " + to_string(yd_id) + " AND tel = '" + tel + "'";
    if (mysql_query(&mysql_con, sql_delete.c_str()) != 0) {
        cout << "删除预约记录失败：" << mysql_error(&mysql_con) << endl;
        mysql_user_rollback();  
        return false;
    }
    int affected = mysql_affected_rows(&mysql_con);//获取最近一次执行的 SQL 语句影响的行数
    if (affected <= 0) {
        cout << "未删除任何记录(yd_id不存在或tel不匹配)" << endl;
        mysql_user_rollback();  
        return false;
    }
    mysql_user_commit();
    return true;
};
bool ser_mysql::mysql_show_sub_ticket(const string& tel, Json::Value& resval){
    string sql = "SELECT "
                 "sub_ticket.yd_id, sub_ticket.tk_id, ticket_info.addr, ticket_info.use_date, sub_ticket.curr_time "
                 "FROM sub_ticket "
                 "JOIN ticket_info ON sub_ticket.tk_id = ticket_info.tk_id "
                 "WHERE sub_ticket.tel = '" + tel + "'";  // 只查当前用户的
    if (mysql_query(&mysql_con, sql.c_str()) != 0) {
        cout << "查询我的预约失败：" << mysql_error(&mysql_con) << endl;
        return false;
    }
    // 处理结果集
    MYSQL_RES* r = mysql_store_result(&mysql_con);
    if (r == NULL){
        return false;
    }
    int num = mysql_num_rows(r);  // 该用户的预约总数
    resval["status"] = "OK";
    resval["num"] = num;
 
    // 遍历结果，存入JSON数组
    for (int i = 0; i < num; i++) {
        MYSQL_ROW row = mysql_fetch_row(r);
        Json::Value tmp;
        tmp["yd_id"] = row[0];    // 预约记录ID（sub_ticket.yd_id）
        tmp["tk_id"] = row[1];    // 项目编号（ticket_info.tk_id）
        tmp["addr"] = row[2];     // 预约地点（ticket_info.addr）
        tmp["use_date"] = row[3]; // 可使用日期（ticket_info.use_date）
        tmp["curr_time"] = row[4];// 预约时间（sub_ticket.curr_time）
        resval["arr"].append(tmp);
    }
    mysql_free_result(r);  
    return true;
};