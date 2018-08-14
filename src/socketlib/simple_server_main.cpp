#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include<iostream>

int main ( int argc, int argv[] )
{
    std::cout << "running....\n";

    try
    {
        // Create the socket
        ServerSocket server ( 30000 );

        while ( true )
        {

            ServerSocket new_sock;
            server.accept ( new_sock );
            std::cout<<"accept"<<std::endl;
            try
            {
                while ( true )
                {
                    std::string data;
                    new_sock >> data;
                    new_sock << data;
                }
            }
            catch ( SocketException& ) {}

        }
    }
    catch ( SocketException& e )
    {
        std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

    return 0;
}