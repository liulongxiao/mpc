//
// Created by llx on 2018/8/11.
//

#ifndef MPC_SYSMESSAGE_H
#define MPC_SYSMESSAGE_H

#define SYS_MESSAGE_BUFFER_LENGTH 1000

typedef char SYS_MESSAGE_STATUS;


struct SysMessage{
    SYS_MESSAGE_STATUS msgType;
    long renewNane;
    bool source;
    char data[SYS_MESSAGE_BUFFER_LENGTH];
};


constexpr const int SYS_MESSAGE_LENGH= sizeof(SysMessage);

#endif //MPC_SYSMESSAGE_H
