//
// Created by llx on 2018/8/8.
//
#include "Page.h"
#include "PageHeader.h"
#include <string>
#include "mmap_helper.h"
#include "../utils/Timer.h"

#include<iostream>
#include<iostream>
using std::string;


PagePtr Page::load(const string &dir, const string &jname, short pageNum, bool isWriting, bool quickMode)
{
    string path = GenPageFullPath(dir, jname, pageNum);
    auto file_exists=FileExists(GenPageFullPath(dir,jname,pageNum));
    void* buffer = LoadPageBuffer(path, JOURNAL_PAGE_SIZE, isWriting, quickMode /*from local then we need to do mlock manually*/);
    if (buffer == nullptr){
        return PagePtr();
        }


    PageHeader* header = (PageHeader*)buffer;

    if(!file_exists)
    {
        /*第一次生成mmap文件，准备好Page头*/
        header->status = JOURNAL_PAGE_STATUS_RAW;
    }

    if (header->status == JOURNAL_PAGE_STATUS_RAW)
    {
        if (!isWriting)
            return PagePtr();
        // initialize
        memcpy(header->journal_name, jname.c_str(), jname.length() + 1);
        header->start_nano = getNanoTime();
        header->close_nano = -1;
        header->page_num = pageNum;
        // then set back status
        header->status = JOURNAL_PAGE_STATUS_INITED;
        // write current frame header version inside.
        header->frame_version = __FRAME_HEADER_VERSION__;
        static_cast<FrameHeader *>(ADDRESS_ADD(header,PAGE_INIT_POSITION))->status=MPC_FRAME_STATUS_RAW;
    }
    else if (header->frame_version > 0 && header->frame_version!= __FRAME_HEADER_VERSION__)
    {
        // if this is an existing page (status!=JOURNAL_PAGE_STATUS_RAW)
        // and frame_version is set
        // (previous we have some page without frame_version set properly)
        // we need to check header version no matter reader or writer.
        std::stringstream ss;
        ss << "page version mismatch: (program)" << __FRAME_HEADER_VERSION__ << " (page)" << header->frame_version;
        throw std::runtime_error(ss.str().c_str());
    }

    PagePtr page = PagePtr(new Page(buffer));
    page->pageNum = pageNum;
    return page;
}


void Page::release(PagePtr pagePtr,bool quickMode){
    std::cout<<"realse"<<std::endl;
    ReleasePageBuffer(pagePtr->getBuffer(),JOURNAL_PAGE_SIZE,quickMode);
    std::cout<<"after realse"<<std::endl;
}