#include "room.hpp"


#ifndef TIME_OUT
#define TIME_OUT 5000
#endif // TIME_OUT

#define ERROR -1
#define TIME_RANOUT 0

#ifdef __unix__
#include <time.h>
#else
#include <chrono>
#include <thread>
#endif // __unix__

#include "serverException.hpp"
 room::room()
 {

 }

 void room::update()
 {
    if(ufds.size())
    {
        int res = poll(ufds.data(),ufds.size(),TIME_OUT);

        if( res == ERROR)
        {
            throw ServerException("Polling has failed");
        }
        else if( res == TIME_RANOUT)
        {
            sendto_all("");
            /* reference for nanosleep @ : http://linux.die.net/man/2/nanosleep*/
            #ifdef __unix__
                timespec t1 , rem;
                t1.tv_sec = 0;
                t1.tv_nsec = 10000000L;
                nanosleep(&t1 , &rem);
            #else
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            #endif
        }
        else
        {
            unsigned int len = ufds.size();
            for(unsigned int it = 0; it < len; it++)
            {
                if(ufds[it].revents & POLLIN)
                {
                    std::string data;
                    if(users[it].recieve(data))
                    {
                        sendto_all(data);
                    }
                    /*else
                    {
                        if(users[it].disconnected())
                        {
                            users.erase(users.begin() + it);
                            ufds.erase(ufds.begin() + it);
                        }
                    }*/
                }
            }
        }
    }// end of if(ufds.size())
 }

 void room::sendto_all(const std::string & data)
 {
     for(auto & client : users)
     {
        client.send_msg(data);
     }
 }


 void room::add_user(int new_sockfd, char new_delim)
 {
    users.push_back(user(new_sockfd, new_delim));
    pollfd new_user;
    new_user.fd = new_sockfd;
    new_user.events = POLLIN | POLLPRI;
    new_user.revents = 0;
    ufds.push_back(new_user);
 }


