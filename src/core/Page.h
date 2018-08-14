
#ifndef MPC_PAGE_H
#define MPC_PAGE_H

#include "FrameHeader.h"
#include "Frame.hpp"
#include "../utils/macros.h"
#include "constants.h"
#include "PageHeader.h"
#include <string>
#include "../utils/Timer.h"
using std::string;

FORWARD_DECLARE_PTR(Page)


/**
 * Page class
 */
class Page
{
private:
    /*PageHeader*/

    PageHeader * page_header;
    /** current frame */
    Frame frame;
    /** address of mmap-file associated with this page */
    void * const buffer;
    /** current position in page */
    int position;
    /** number index of current frame in the page */
    int frameNum;
    /** number of the page for the journal */
    short pageNum;

    /** private constructor */
    Page(void *buffer) : buffer(buffer), frame(ADDRESS_ADD(buffer, PAGE_INIT_POSITION)), position(PAGE_INIT_POSITION), frameNum(0) {
        std::cout<<"page buffer :"<<uintptr_t (buffer)<<std::endl;
        std::cout<<"frame header init :"<<uintptr_t (frame.get_address())<<std::endl;
    }

private:
    /** internal usage */
    inline FH_TYPE_STATUS getCurStatus() const
    {
        return frame.getStatus();
    }

public:
    /** get page buffer */
    inline void* getBuffer() { return buffer; }
    /** get current page number */
    inline short getPageNum() const { return pageNum; };


    int getFrameCounts(){
        return page_header->frame_num;
    }

    bool passFrame()
    {
        if(isAtPageEnd())
            return false;
        position += frame.next();
        frameNum += 1;
        return true;
    }

    /** setup the page when finished */
    void finishPage(){
        PageHeader* header = (PageHeader*)buffer;
        header->close_nano = getNanoTime();
        header->frame_num = frameNum;
        header->last_pos = position;
    }
    /** get writable frame address (enough space & clean)*/
    void *locateWritableFrame(){
        while(frame.getStatus()!=MPC_FRAME_STATUS_RAW){
            if(!passFrame())
                break;
        }
        if(frame.getStatus()!=MPC_FRAME_STATUS_RAW)
            return nullptr;
        else
            return frame.get_address();
    }


    /** get wrote frame address
     * return nullptr if no more frame */
    void *locateReadableFrame(){
        if(frame.getStatus()==MPC_FRAME_STATUS_WRITTEN)
            return frame.get_address();
        else
            return nullptr;
    }

    /** current page is end */
    bool isAtPageEnd() const{
        return position + PAGE_MIN_HEADROOM >= JOURNAL_PAGE_SIZE;
    }


    /** pass frame to nano_time */
    void passToTime(long time){


    }

    int curFrameNum()const{
        return frameNum;
    }


    int curFramePos()const{
        return position;
    }

    static PagePtr load(const string &dir, const string &jname, short pageNum, bool isWriting, bool quickMode);
    static void release(PagePtr pagePtr,bool quickMode);

};

#endif //MPC_PAGE_H
