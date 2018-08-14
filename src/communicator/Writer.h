//
// Created by root on 18-8-10.
//

#ifndef MPC_WRITER_H
#define MPC_WRITER_H
#include "../core/Journal.h"
#include "../utils/macros.h"
#include <map>
#include "communicator.h"
using std::map;

FORWARD_DECLARE_PTR(Writer)
class Writer: public communicator {
private:
    SysJournalPtr system_journal;
    JournalPtr journal;
    string name;
    Writer():frame(nullptr){}


public:

    int Regist()override{
        Massage sendmsg=Massage::buildMsg(name.c_str(),MPC_MSG_USER_REG_WRITER,journal->getJournalName().c_str());
        auto recvmsg=sendMessageAndBlock(sendmsg);
        if(recvmsg.getResponseStatus()!=0) {
            std::cout<<__FUNCTION__<<" ERROR:"<<recvmsg.getResponseStatus()<<std::endl;
        }else{
            setComIdx(recvmsg.getIdx());
        }
        return recvmsg.getResponseStatus();
    }

    int UnRegist()override {
        Massage sendmsg=Massage::buildMsg(name.c_str(),MPC_MSG_USER_UNREG_WRITER);
        auto recvmsg=sendMessageAndBlock(sendmsg);
        if(recvmsg.getResponseStatus()!=0) {
            std::cout<<__FUNCTION__<<" ERROR:"<<recvmsg.getResponseStatus()<<std::endl;
        }
        return recvmsg.getResponseStatus();
    }



    virtual void WriteFrame(void * data,int len);
    static WriterPtr create(const char * dir,const char * jname);
    Frame frame;
    string getName()const{
        return name;
    }

    ~Writer(){UnRegist();}

};


#endif //MPC_WRITER_H
