//
// Created by llx on 2018/8/8.
//

#include "MpcServer.h"
#include"../utils/utils.h"
#include "SockComConstant.h"
#include "../socketlib/SocketException.h"
#include <thread>
#include<functional>
#include<iostream>
MpcServerPtr MpcServer::serverInstance=nullptr;



void MpcServer::run() {
    while (run_flag) {
        ServerSocket com_sock;
        server.accept(com_sock);
        std::cout<<"accept sock"<<std::endl;
        try {
            std::string raw_msg;
            com_sock >> raw_msg;
            std::cout<<"receive msg :"<<raw_msg<<std::endl;
            /*handle msg*/
            auto msg = Massage::buildMsg(raw_msg.c_str());
            int rescode, idx;
            switch (msg.getMsgType()) {
                case MPC_MSG_USER_REG_READER:
                    rescode = RegistReader(msg.getClientName().c_str(), idx);
                    msg.setResponseStatus(rescode);
                    msg.setIdx(idx);
                    com_sock << msg.dumpMsg();
                    break;
                case MPC_MSG_USER_REG_WRITER:
                    rescode = RegistWriter(msg.getClientName().c_str(), msg.getJournalName().c_str(), idx);
                    msg.setResponseStatus(rescode);
                    msg.setIdx(idx);
                    com_sock << msg.dumpMsg();
                    break;
                case MPC_MSG_USER_READER_ADD_JOURNAL:
                    rescode = ReaderAddJournal(msg.getClientName().c_str(), msg.getJournalName().c_str());
                    msg.setResponseStatus(rescode);
                    com_sock << msg.dumpMsg();
                    break;
                case MPC_MSG_USER_READER_REMOVE_JOURNAL:
                    rescode = ReaderRemoveJournal(msg.getClientName().c_str(), msg.getJournalName().c_str());
                    msg.setResponseStatus(rescode);
                    com_sock << msg.dumpMsg();
                    break;
                case MPC_MSG_USER_UNREG_READER:
                    rescode = UnregReader(msg.getClientName().c_str());
                    msg.setResponseStatus(rescode);
                    com_sock << msg.dumpMsg();
                    break;
                case MPC_MSG_USER_UNREG_WRITER:
                    rescode = UnregWriter(msg.getClientName().c_str());
                    msg.setResponseStatus(rescode);
                    com_sock << msg.dumpMsg();
                    break;
            }
        }
        catch (SocketException &e) {
            std::cout << e.description() << std::endl;
        }

    }
}
//void MpcServer::Start(){
//    std::thread workingThread(std::bind(&MpcServer::run,this));
//}