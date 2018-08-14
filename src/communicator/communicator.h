//
// Created by llx on 2018/8/13.
//

#ifndef MPC_COMMUNICATOR_H
#define MPC_COMMUNICATOR_H
#include "../socketlib/ClientSocket.h"
#include "../service/SockComConstant.h"
#include "../socketlib/SocketException.h"
#include <iostream>
#include <string>
class communicator {
public:
    communicator():comIdx(-1){}

    Massage sendMessageAndBlock(const Massage&msg){
        ClientSocket sock("localhost",32013);
        std::string reply;
        try{
        sock << msg.dumpMsg();
        sock >> reply;
        std::cout<<"reply:"<<reply<<std::endl;
            }
        catch(SocketException & e){
            std::cout<<e.description()<<std::endl;
        }

        return Massage::buildMsg(reply.c_str());
    }

    virtual int  Regist()=0;

    virtual int UnRegist()=0;



    void setComIdx(int idx){
        comIdx=idx;
    }

    int getComIdx()const {
        return comIdx;
    }



private:
    int comIdx;
   // ClientSocket sock;
};


#endif //MPC_COMMUNICATOR_H
