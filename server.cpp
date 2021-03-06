#include "server.hpp"
#include "room.hpp"
#include "serverException.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <memory>
/*'

    NOTE: inorder to change the KEEP-ALIVE for TCP, you can do the following :

    echo 600 > /proc/sys/net/ipv4/tcp_keepalive_time

    echo 60 > /proc/sys/net/ipv4/tcp_keepalive_intvl

    echo 20 > /proc/sys/net/ipv4/tcp_keepalive_probes

*/

#ifndef MAXPENDING
#define MAXPENDING 5
#endif
int make_socket_non_blocking(int sfd)
{
  int flags;
  flags = fcntl (sfd, F_GETFL, 0);
  if (flags == -1)
      return -1;
  flags |= O_NONBLOCK;
  return fcntl (sfd, F_SETFL, flags);
}

server::server( int Port )
{
    if((servfd = socket(PF_INET,SOCK_STREAM, 0)) < 0)
        throw ServerException("server error : could not create socket");

     sockaddr_in ServAddr;
     std::memset(&ServAddr, 0, sizeof(ServAddr));
     ServAddr.sin_family = AF_INET; /* Internet address family */
     ServAddr.sin_addr.s_addr = INADDR_ANY; /* Any incoming interface */
     ServAddr.sin_port = htons(Port); /* Local port */


     if (bind(servfd, (sockaddr *)&ServAddr, sizeof(ServAddr)) < 0)
        throw ServerException("server error : bind");

    if( make_socket_non_blocking(servfd) == -1)
        throw ServerException("server error : servfd could not be set to non blocking");

    // you can check max backlog @  /proc/sys/net/core/somaxconn
     if(listen(servfd, MAXPENDING) < 0)
        throw ServerException("server error : listen");

}


void server::run()
{
    struct sockaddr_in client_addr;
    unsigned int client_len = sizeof(sockaddr_in);
    room r1;
    keep_alive = true;
    while(keep_alive)
    {
        int clientfd;
        /* wait for accepting clients */
        if ((clientfd = accept(servfd, (sockaddr *) &client_addr,&client_len)) >= 0)
        {
            if(make_socket_non_blocking(clientfd) == -1 )
            {
                std::string err("RESP couldn't make non-blocking for ip:");
                err.append(inet_ntoa(client_addr.sin_addr));
                panel.notify(err);
            }
            else
            {
                r1.add_user(clientfd, '@');

                /* update the panel that a new connection occured */
                std::string info("RESP The following ip has connected:");
                info.append(inet_ntoa(client_addr.sin_addr));
                panel.notify(info);
            }

        }


        r1.update();

        std::shared_ptr<std::string> req = panel.receive();
        if(req)
        {
            /* kills the server */
            if(req->find("KILL") != std::string::npos)
            {
                keep_alive = 0;
                panel.notify("KILLT");
            }
        }
    }//while(keep_alive)
    panel.notify("KILLT");
}

server::~server()
{
    panel.notify("KILLT");
    close(servfd);
}
