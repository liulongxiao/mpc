
#include "Journal.h"
#include "mmap_helper.h"

#include<iostream>

JournalPtr Journal::create(const string &dir, const string &jname,bool isWriting,bool quickMode)
{
    JournalPtr jp = JournalPtr(new Journal(isWriting));
    jp->directory = dir;
    jp->jname = jname;
    jp->quickMode=quickMode;

    /*准备第一次要用到的page*/
    jp->loadNextPage();

    return jp;
}


PagePtr Journal::newPage() {
   return  Page::load(directory,jname,curPageNum+1,isWriting,quickMode);
}



void Journal::loadNextPage()
{

    if (curPage.get() == nullptr)
    {
        curPage = newPage();
    }
    else
    {   // allocate new page
        PagePtr newpage = newPage();
        // stop current page
        if (isWriting)
        {
            curPage->finishPage();
        }
        ReleasePageBuffer(curPage->getBuffer(), JOURNAL_PAGE_SIZE,quickMode);
        // reset current page
        curPage = newpage;
    }
    ++curPageNum;
}

void* Journal::locateFrame()
{

    // if writing, we need an empty frame
    // if reading, we need an applicable frame
    if (isWriting)
    {
        void* frame = curPage->locateWritableFrame();
        while (frame == nullptr)
        {
            loadNextPage();
            frame = curPage->locateWritableFrame();
        }
        return frame;
    }
    else
        return curPage->locateReadableFrame();
};


SysJournalPtr SysJournal::sysjournal=nullptr;