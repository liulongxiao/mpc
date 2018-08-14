//
// Created by llx on 2018/8/13.
//

#ifndef MPC_SOCKCOMCONSTANT_H
#define MPC_SOCKCOMCONSTANT_H
#include "../utils/utils.h"
#include <string>
#include <iostream>
#define MPC_MSG_USER_REG_READER 1
#define MPC_MSG_USER_REG_WRITER 2
#define MPC_MSG_USER_UNREG_READER 3
#define MPC_MSG_USER_UNREG_WRITER 4
#define MPC_MSG_USER_READER_ADD_JOURNAL 5
#define MPC_MSG_USER_READER_REMOVE_JOURNAL 6

/**
 *
 * Msg:    |senderName|MsgType|journalName(optional)|idx(used By Server to notice comidx)|
 *
 !*/


class Massage{
private:
    std::string raw_msg;
    Massage():msgType(-1),idx(-1){}
    std::string clientName;
    std::string journalName;
    int msgType;
    int idx;
    int response_status;
public:
    std::string dumpMsg()const{
        return raw_msg;
    }

    std::string getClientName()const{
        return clientName;
    }
    std::string getJournalName()const {
        return journalName;
    }
    int getMsgType()const {
        return msgType;
    }

    int getIdx()const
    {
        return idx;
    }

    int getResponseStatus()const{
        return response_status;
    }


    void setIdx(int idx_){
        idx=idx_;
        rebuild_raw_msg();
    }
    void setResponseStatus(int res){
        response_status=res;
        rebuild_raw_msg();

    }
    void rebuild_raw_msg(){
        raw_msg=clientName+"|"+stringulate(msgType)+"|"+journalName+"|"+stringulate(idx)+"|"+stringulate(response_status);
    }




    static Massage buildMsg(const char * clientName,int msgType,const char * jname= "NULL",int idx=-1,int response_status=-1){
        Massage msg;
        msg.clientName=std::string(clientName);
        msg.journalName=std::string(jname);
        msg.idx=idx;
        msg.msgType=msgType;
        msg.response_status=response_status;
        msg.raw_msg=msg.clientName+"|"+stringulate(msg.msgType)+"|"+msg.journalName+"|"+stringulate(idx)+"|"+stringulate(msg.response_status);

        return msg;
    }
    static Massage buildMsg(const char * raw_msg){
        Massage msg;
        msg.raw_msg=std::string(raw_msg);
        auto parses_vec=split(msg.raw_msg,"\\|");
        msg.clientName=parses_vec[0];
        msg.msgType=atoi(parses_vec[1].c_str());
        msg.journalName=parses_vec[2];
        msg.idx=atoi(parses_vec[3].c_str());
        msg.response_status=atoi(parses_vec[4].c_str());
        return msg;
    }

};














#endif //MPC_SOCKCOMCONSTANT_H
