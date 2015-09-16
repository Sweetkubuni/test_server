
control::control()
{
    keep_alive = true;
    th = unique_ptr<std::thread>(new thread(bind(&consumer::run, this)));
}

shared_ptr<std::string> control::receive()
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
       std::cout << " ___  _          _     ___                            _     ___ " << '\n';
       std::cout << "|  _>| |_  ___ _| |_  / __> ___  _ _  _ _  ___  _ _  / |   |   |" << '\n';
       std::cout << "| <__| . |<_> | | |   \__ \/ ._>| '_>| | |/ ._>| '_> | | _ | / |" << '\n';
       std::cout << "`___/|_|_|<___| |_|   <___/\___.|_|  |__/ \___.|_|   |_|<_>`___'" << '\n';
       std::shared_ptr<std::string> resp = out_mq.pop();
       while(resp)
       {
           std::string command = resp->substr(0,4);
           if(command.size() > 0)
           {
               if(command.compare("KILLT") == 0)
                    keep_alive = false;
               else if(command.compare("RESP") == 0)
                    std::cout <<  resp->substr(4, std::string:npos) << "\n";
               else
                    std::cout << "Unexpected Command!\n";
           }
           resp = out_mq.pop();
       }
       std::cout << "Enter Command:";
       std::cin.getline(line, 256, '\n');
       std::string msg(line);
       in_mq.push(msg);
    }
}

control::~control()
{
    th->join();
}
