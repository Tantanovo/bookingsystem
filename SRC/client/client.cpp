#include"client.hpp"



int main(){
    connect_client client;
    if(!client.init_client()){
        perror("init client err");
        return -1;
    }
    return 0;
}