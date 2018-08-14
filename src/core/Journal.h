
#ifndef MPC_JOURNAL_H
#define MPC_JOURNAL_H

#include "../utils/macros.h"
#include "Page.h"
#include "SysMessage.h"
#include <string>



using std::string;


FORWARD_DECLARE_PTR(Journal);

class Journal
{
protected:
    /** basic information, directory path */
    string  directory;
    /** basic information, journal short name */
    string  jname;
    /** !!for performance only, cache pageProvider->isWriter() */
    bool    isWriting;

    bool    quickMode;


    /** private constructor, make create the only builder */

    /*当前按page的编号，若为-1则当前page为nullptr*/
    int curPageNum;

    /*new page helper*/
    PagePtr newPage();



    /** current page in use */
    PagePtr curPage;

protected:
    Journal(bool isWriting):curPageNum(-1),isWriting(isWriting){};
    void * getBuffer(){
        return curPage->getBuffer();
    }


public:

    ~Journal(){
        if(curPage!= nullptr)
        {
            Page::release(curPage,quickMode);
        }
    }
    /** the only entrance of creating a Journal */
    static JournalPtr create(const string& dir, const string& jname,bool isWriting,bool quickMode);


    void passFrame(){
        if(curPage->isAtPageEnd())
            loadNextPage();
        else
            curPage->passFrame();
    }

    /** get frame address return nullptr if no available */
    void*   locateFrame();

    /** load next page, current page will be released if not empty */
    void    loadNextPage();
    /** get current page number */



    short   getCurPageNum() const;
    /** get journal short name */
    string  getJournalName() const;

    int     getCurFrameId()const{
        return curPage->curFrameNum();
    }

    int     getCurFramePos()const{
        return curPage->curFramePos();
    }
};

FORWARD_DECLARE_PTR(SysJournal)
class SysJournal:public Journal{

private:
    static SysJournalPtr sysjournal;
    SysJournal():Journal(true){


        directory = SYSTEM_JOURNAL_DIR;
        jname = SYSTEM_JOURNAL_NAME;
        quickMode=false;

        /*准备第一次要用到的page*/
        loadNextPage();
    }
public:
    static SysJournalPtr getInstance(){
        if(sysjournal!= nullptr)
            return sysjournal;
        else{
            sysjournal=SysJournalPtr(new SysJournal);
            return sysjournal;
        }
    }
    void * getMsgBuffer(int idx){
        return ADDRESS_ADD(Journal::getBuffer(),idx*SYS_MESSAGE_LENGH);
    }
};



inline short Journal::getCurPageNum() const
{
    return curPage->getPageNum();
}

inline string Journal::getJournalName() const
{
    return jname;
}


#endif //MPC_JOURNAL_H
