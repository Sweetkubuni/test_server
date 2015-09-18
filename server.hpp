
/*
    @file: server.hpp

    @purpose : Handles a group of clients connected to the server;

*/

#ifndef SERVER_HPP
#define SERVER_HPP
#include "control.hpp"
class server
{
    int servfd;
    bool keep_alive;
    control panel;
    public:
    server( int Port );
    ~server();

    void run();
};
#endif // SERVER_HPP
