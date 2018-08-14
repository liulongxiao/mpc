//
// Created by root on 18-8-10.
//

#include "Writer.h"
#include "../core/Frame.hpp"
#include "../utils/macros.h"
#include "../core/Journal.h"

void Writer::WriteFrame(void * data,int len){

    frame.set_address(journal->locateFrame());
    frame.setData(data,len);
    frame.setNano(getNanoTime());
    frame.setStatusWritten();
}

WriterPtr Writer::create(const char * dir,const char * jname){
    auto writer=WriterPtr(new Writer);
    writer->system_journal=SysJournal::getInstance();
    writer->journal=Journal::create(dir,jname,true,false);
    writer->name=string(jname);

    if(writer->Regist()==0)
        return writer;
    else return nullptr;
}

