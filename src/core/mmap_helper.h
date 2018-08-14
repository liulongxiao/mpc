//
// Created by llx on 2018/8/8.
//

#ifndef MPC_MMAP_HELPER_H
#define MPC_MMAP_HELPER_H

#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sstream>
#include <string>
#include<unistd.h>
#include "../utils/macros.h"


using std::string;


void* LoadPageBuffer(const string& path, int size, bool isWriting, bool quickMode);
void ReleasePageBuffer(void *buffer, int size, bool quickMode);
bool FileExists(const string& filename);

inline string GenPageFileName(const string &jname, short pageNum)
{
    return string(MPC_JOURNAL_NAME_PREFIX) + "." + jname + "." + std::to_string(pageNum) + "." + string(MPC_JOURNAL_NAME_ENDING);
}


inline string GenPageFullPath(const string& dir, const string& jname, short pageNum)
{
    std::stringstream ss;
    ss << dir << "/" << GenPageFileName(jname, pageNum);
    return ss.str();
}


#endif //MPC_MMAP_HELPER_H
