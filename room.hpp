
#ifndef ROOM_HPP
#define ROOM_HPP
#ifdef __unix__
#include <poll.h>
#endif // __unix__
#include "user.hpp"
class room
{
    private:
        std::vector<user> users;
        std::vector<pollfd> ufds;
        void sendto_all(const std::string & data);
    public:
        room();
        void update();
        void add_user(int new_sockfd, char new_delim);
        void user_ping()
};
#endif // ROOM_HPP
