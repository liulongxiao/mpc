//
// Created by llx on 2018/8/12.
//

#ifndef MPC_SERVERSOCKET_H
#define MPC_SERVERSOCKET_H

#include "Socket.h"


class ServerSocket : private Socket
{
public:

    ServerSocket ( int port );
    ServerSocket (){};
    virtual ~ServerSocket();

    const ServerSocket& operator << ( const std::string& ) const;
    const ServerSocket& operator >> ( std::string& ) const;

    void accept ( ServerSocket& );

};


#endif //MPC_SERVERSOCKET_H
