#include "control.hpp"
#include <iostream>
control::control()
{
    keep_alive = true;
    th = std::unique_ptr<std::thread>(new std::thread(std::bind(&control::run, this)));
}

std::shared_ptr<std::string> control::receive()
{
    return in_mq.pop();
}

void control::notify(const std::string & msg)
{
    out_mq.push(msg);
}

void control::run()
{
    char line[256];
    while(keep_alive)
    {
        /* clear screen*/
       std::cout << "\033[2J";
       
       /* Welcome Screen */
       std::shared_ptr<std::string> resp = out_mq.pop();
       if(resp)
       {
           std::string command = resp->substr(0,4);
           std::cout << command<<'\n';
           if(command.size() > 0)
           {
               if(command.find("KILLT") != std::string::npos )
               {
                  std::cout << "Server 1.0 [ ^_^ ] bye!\n";  
                  keep_alive = false; 
               }
               else if(command.compare("RESP") == 0)
               {
                   std::cout << "Server 1.0 [ O_O ] Server Responded!!\n";
                    std::cout <<  resp->substr(4, std::string::npos) << "\n";
               }
               else
               {
                    std::cout << "Server 1.0 [ >_< ] uff!\n";
                    std::cout << "Unexpected Command!\n";
               }
           }
           else
           {
              std::cout << "Server 1.0 [^_^ ]\n"; 
           }
           resp = out_mq.pop();
       }
       std::cout << "Enter Command:";
       std::cin.getline(line, 256, '\n');
       std::string msg(line);
       if( msg.compare("update") == 0)
           continue;
       in_mq.push(msg);
    }
}

control::~control()
{
    th->join();
}
