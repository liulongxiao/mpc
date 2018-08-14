//
// Created by llx on 2018/8/13.
//
#include "MpcServer.h"
#include<iostream>
using std::cout;
using std::endl;
int main(){
    auto service=MpcServer::getInstance();
   // service->run();
    cout<<"after create instance"<<endl;
    service->Start();
    service->run();

}