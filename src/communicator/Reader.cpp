//
// Created by llx on 2018/8/8.
//

#include "Reader.h"
ReaderPtr Reader::create(const char * readerName){

    auto reader=ReaderPtr(new Reader);
    reader->system_journal=SysJournal::getInstance();
    reader->name=string(readerName);
    reader->curJournal=nullptr;
    if(reader->Regist()==0)
        return reader;
    else return nullptr;
}

void * Reader::readFrame(){
    long  minNano = -1;
    void* res_address = nullptr;
    for (auto& iter: journals)
    {
        auto journal=iter.second;
        FrameHeader* header = (FrameHeader*)(journal->locateFrame());
        if (header != nullptr)
        {
            long nano = header->nano;
            if (minNano == -1 || nano < minNano)
            {
                minNano = nano;
                res_address = header;
                curJournal = journal;
            }
        }
    }

    if (res_address != nullptr)
    {
        curJournal->passFrame();
        return res_address;
    }
    else
    {
        return nullptr;
    }

//    FrameHeader* header = (FrameHeader*)(journals["testj5"]->locateFrame());
//    if (header != nullptr){
//        long curtime=getNanoTime();
//        std::cout<<"internal time duration :"<<curtime-header->nano<<std::endl;
//        journals["testj5"]->passFrame();
//        return FramePtr(new Frame((void*)(header)));
//    }
//    else{
//        return FramePtr();
//    }






}