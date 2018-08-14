//
// Created by llx on 2018/8/12.
//

#ifndef MPC_CLIENTSOCKET_H
#define MPC_CLIENTSOCKET_H


#include "Socket.h"


class ClientSocket : private Socket
{
 public:

  ClientSocket ( std::string host, int port );
  virtual ~ClientSocket(){};

  const ClientSocket& operator << ( const std::string& ) const;
  const ClientSocket& operator >> ( std::string& ) const;

};




#endif //MPC_CLIENTSOCKET_H
