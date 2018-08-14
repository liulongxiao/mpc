//
// Created by llx on 2018/8/8.
//

#ifndef MPC_MPCSERVER_H
#define MPC_MPCSERVER_H
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include "../utils/macros.h"
#include "../core/Journal.h"
#include "../socketlib/ServerSocket.h"

using std::map;
using std::string;
using std::vector;

typedef int MPC_REGIST_READER_STATUS;

typedef int MPC_READER_ADDJOURNAL_STATUS;

typedef int MPC_READER_REMOVE_JOURNAL_STATUS;

typedef int MPC_UNREGIST_READER_STATUS;

typedef int MPC_REGIST_WRITER_STATUS;

typedef int MPC_UNREGIST_WRITER_STATUS;

#define MPC_REGIST_READER_SUCCESS 0
#define MPC_REGIST_READER_DUPLICATE 1

#define MPC_READER_ADDJOURNAL_SUCCESS 0
#define MPC_READER_ADDJOURNAL_READER_NOT_EXISTS 1
#define MPC_READER_ADDJOURNAL_JOURNAL_DUPLICATE 2

#define MPC_READER_REMOVE_JOURNAL_SUCCESS 0
#define MPC_READER_REMOVE_JOURNAL_READER_NOT_EXISTS 1
#define MPC_READER_REMOVE_JOURNAL_JOURNAL_NOT_EXISTS  2

#define MPC_REGIST_WRITER_SUCCESS 0
#define MPC_REGIST_WRITER_WRITER_EXISTS 1
#define MPC_REGIST_WRITER_DUPLICATE_USE_OF_JOURNAL  2

#define MPC_UNREGIST_READER_SUCCESS 0
#define MPC_UNREGIST_READER_NOT_EXISTS 1


#define MPC_UNREGIST_WRITER_SUCCESS 0
#define MPC_UNREGIST_WRITER_NOTE_EXISTS 1

#define MPC_USER_NUM_OVERFLOW 99


#define MPC_SysJournalMaxUsers 100

#define MPC_CONNECT_PORT 32013


FORWARD_DECLARE_PTR(MpcServer)

class MpcServer {
private:
    map<string,vector<string>> readerJournals;
    map<string,string> writerJournal;
    map<string,int> sysJournalComIdxMap;
    SysJournalPtr system_journal;
    ServerSocket server;
    bool run_flag;
    MpcServer():server (MPC_CONNECT_PORT),run_flag(false){std::cout<<"create"<<std::endl;}/*singleton*/
    static MpcServerPtr serverInstance;
public:


    void run();

    void Stop(){
        run_flag=false;
    }
    void Start(){
        run_flag=true;
    }



    static MpcServerPtr getInstance(){
        if(serverInstance!=nullptr)
            return serverInstance;
        else{
            serverInstance=MpcServerPtr(new MpcServer);
            std::cout<<serverInstance->run_flag<<std::endl;
            serverInstance->system_journal=SysJournal::getInstance();
            return serverInstance;
        }
    }

private:

    void ProcessSockMsg(){}

    bool registCom(const char * clientName,int & idx){
        for(int i=0;i!=MPC_SysJournalMaxUsers;++i) {
            if(find_if(sysJournalComIdxMap.cbegin(), sysJournalComIdxMap.cend(),[&i]( const std::pair<string,int>&a) { return a.second == i;})==sysJournalComIdxMap.end())
            {
                sysJournalComIdxMap[string(clientName)]=i;
                idx=i;
                return true;
            }
        }
        return false;
    }

    void unRegistCom(const char * clientName){
        sysJournalComIdxMap.erase(sysJournalComIdxMap.find(string(clientName)));
    }


    MPC_REGIST_READER_STATUS RegistReader(const char * readerName,int & idx){
        if(readerJournals.find(string(readerName))!=readerJournals.end()){
            return MPC_REGIST_READER_DUPLICATE;
        }else{

            if(!registCom(readerName,idx)){
                return MPC_USER_NUM_OVERFLOW;
            }else{
                readerJournals[string(readerName)];
                return MPC_REGIST_READER_SUCCESS;
            }
        }
    }

    MPC_READER_ADDJOURNAL_STATUS ReaderAddJournal(const char * readerName,const char * jname){
        auto iter=readerJournals.find(string(readerName));
        if(iter==readerJournals.end())
            return MPC_READER_ADDJOURNAL_READER_NOT_EXISTS;
        else{
            if(std::find(iter->second.begin(),iter->second.end(),string(jname))!=iter->second.end())
                return MPC_READER_ADDJOURNAL_JOURNAL_DUPLICATE;
            else {
                iter->second.push_back(string(jname));
                return MPC_READER_ADDJOURNAL_SUCCESS;
            }
        }
    }

    MPC_READER_REMOVE_JOURNAL_STATUS ReaderRemoveJournal(const char * readerName,const char * jname){
        auto iter=readerJournals.find(string(readerName));
        if(iter==readerJournals.end()){
            return MPC_READER_REMOVE_JOURNAL_READER_NOT_EXISTS;
        }else{
            auto j=std::find(iter->second.begin(),iter->second.end(),string(jname));
            if(j==iter->second.end()){
                return MPC_READER_REMOVE_JOURNAL_JOURNAL_NOT_EXISTS;
            }else{
                iter->second.erase(j);
                return MPC_READER_REMOVE_JOURNAL_SUCCESS;

            }
        }
    }

    MPC_REGIST_WRITER_STATUS RegistWriter(const char * writerName,const char * jname,int & idx){
        if(writerJournal.find(string(writerName))!=writerJournal.end())
            return MPC_REGIST_WRITER_WRITER_EXISTS;
        for(auto iter=writerJournal.begin();iter!=writerJournal.end();++iter)
        {
            if(iter->second==string(jname))
                return MPC_REGIST_WRITER_DUPLICATE_USE_OF_JOURNAL;
        }

        if(!registCom(writerName,idx)){
            return MPC_USER_NUM_OVERFLOW;
        }
        else{
        writerJournal[string(writerName)]=string(jname);
        return MPC_REGIST_WRITER_SUCCESS;
        }
    }

    MPC_UNREGIST_READER_STATUS UnregReader(const char * readerName){
        auto  iter=readerJournals.find(string(readerName));
        if(iter==readerJournals.end()){
            return MPC_UNREGIST_READER_NOT_EXISTS;
        }else{
            readerJournals.erase(iter);
            unRegistCom(readerName);
            return MPC_UNREGIST_READER_SUCCESS;
        }

    }

    MPC_UNREGIST_WRITER_STATUS UnregWriter(const char * writerName){
        auto  iter=writerJournal.find(string(writerName));
        if(iter==writerJournal.end()){
            return MPC_UNREGIST_WRITER_NOTE_EXISTS;
        }else{
            writerJournal.erase(iter);
            unRegistCom(writerName);
            return MPC_UNREGIST_WRITER_SUCCESS;
        }

    }

};


#endif //MPC_MPCSERVER_H
