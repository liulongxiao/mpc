//
// Created by llx on 2018/8/8.
//

#ifndef MPC_READER_H
#define MPC_READER_H

#include "com_types.h"
#include "../core/Journal.h"
#include<map>
#include <string>
#include "communicator.h"
#include <iostream>
using std::string;
using std::map;

FORWARD_DECLARE_PTR(Reader)
class Reader: public communicator {

public:
    void *  readFrame();
    void addJournal(const char * dir,const char * jname){
        Massage sendmsg=Massage::buildMsg(name.c_str(),MPC_MSG_USER_READER_ADD_JOURNAL,jname);
        auto recvmsg=sendMessageAndBlock(sendmsg);
        if(recvmsg.getResponseStatus()!=0) {
            std::cout<<__FUNCTION__<<" ERROR:"<<recvmsg.getResponseStatus()<<std::endl;
            return;
        }
        if(journals.find(string(jname))==journals.end()){
            journals[jname]=Journal::create(dir,jname,false,false);
        }
    }
    void removeJournal(const char * dir,const char * jname){
        Massage sendmsg=Massage::buildMsg(name.c_str(),MPC_MSG_USER_READER_REMOVE_JOURNAL,jname);
        auto recvmsg=sendMessageAndBlock(sendmsg);
        if(recvmsg.getResponseStatus()!=0) {
            std::cout<<__FUNCTION__<<" ERROR:"<<recvmsg.getResponseStatus()<<std::endl;
            return;
        }
        auto iter=journals.find(string(jname));
        if(iter!=journals.end()){
            journals.erase(iter);
        }
    }


    int Regist()override{
        Massage sendmsg=Massage::buildMsg(name.c_str(),MPC_MSG_USER_REG_READER);
        auto recvmsg=sendMessageAndBlock(sendmsg);
        if(recvmsg.getResponseStatus()!=0) {
            std::cout<<__FUNCTION__<<" ERROR:"<<recvmsg.getResponseStatus()<<std::endl;
        }else{
            setComIdx(recvmsg.getIdx());
        }
        return recvmsg.getResponseStatus();
    }

    int UnRegist()override {
        Massage sendmsg=Massage::buildMsg(name.c_str(),MPC_MSG_USER_UNREG_READER);
        auto recvmsg=sendMessageAndBlock(sendmsg);
        if(recvmsg.getResponseStatus()!=0) {
            std::cout<<__FUNCTION__<<" ERROR:"<<recvmsg.getResponseStatus()<<std::endl;
        }
        return recvmsg.getResponseStatus();
    }


    static ReaderPtr create(const char * readerName);

    string getName()const{
        return name;
    }

    ~Reader(){
        UnRegist();
    }

private:


    Reader(){};
    SysJournalPtr system_journal;
    map<string,JournalPtr> journals;
    string name;
    JournalPtr curJournal;

};


#endif //MPC_READER_H
