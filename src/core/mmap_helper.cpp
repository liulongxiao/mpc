//
// Created by root on 18-8-9.
//
#include "mmap_helper.h"
#include <iostream>

void* LoadPageBuffer(const string& path, int size, bool isWriting, bool quickMode)
{
    int fd = open(path.c_str(), (isWriting) ? (O_RDWR | O_CREAT) : O_RDONLY, (mode_t)0600);

    if (fd < 0)
    {
        if (!isWriting)
            return nullptr;
        perror("Cannot create/write the file");
        exit(EXIT_FAILURE);
    }

    if (/*!quickMode &&*/ isWriting)
    {
        if (lseek(fd, size-1, SEEK_SET) == -1)
        {
            close(fd);
            perror("Error calling lseek() to 'stretch' the file");
            exit(EXIT_FAILURE);
        }
        if (write(fd, "", 1) == -1)
        {
            close(fd);
            perror("Error writing last byte of the file");
            exit(EXIT_FAILURE);
        }
    }

    void* buffer = mmap(0, size, (isWriting) ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, fd, 0);

    if (buffer == MAP_FAILED)
    {
        close(fd);
        perror("Error mapping file to buffer");
        exit(EXIT_FAILURE);
    }

    if (!quickMode && madvise(buffer, size, MADV_RANDOM) != 0 && mlock(buffer, size) != 0)
    {
        munmap(buffer, size);
        close(fd);
        perror("Error in madvise or mlock");
        exit(EXIT_FAILURE);
    }

    close(fd);
    return buffer;
}

void ReleasePageBuffer(void *buffer, int size, bool quickMode)
{
    //unlock and unmap
    if (!quickMode && munlock(buffer, size) != 0)
    {
        perror("ERROR in munlock");
        exit(EXIT_FAILURE);
    }

    if(munmap(buffer, size)!=0)
    {
        perror("ERROR in munmap");
        exit(EXIT_FAILURE);
    }
}

bool FileExists(const string& filename)
{
    int fd = open(filename.c_str(), O_RDONLY, (mode_t)0600);
    if (fd >= 0)
    {
        close(fd);
        return true;
    }
    return false;
}

