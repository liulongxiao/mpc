//
// Created by llx on 2018/8/12.
//

#ifndef MPC_SOCKETEXCEPTION_H
#define MPC_SOCKETEXCEPTION_H
// SocketException class


#include <string>

class SocketException
{
public:
    SocketException ( std::string s ) : m_s ( s ) {};
    ~SocketException (){};

    std::string description() { return m_s; }

private:

    std::string m_s;

};

#endif //MPC_SOCKETEXCEPTION_H
