#include "server.hpp"
#include "serverexception.hpp"
#include <iostream>
using namespace std;
int main(int arg, char ** argv)
{
    try
    {
        if( arg ==  2 )
        {
            server chat( atoi(argv[1]));
            chat.run();
        }
    }
    catch( ServerException & err)
    {
       ofstream log;

       log.open("error.txt", ios::out | ios::app );

       if(log.is_open())
       {
            log << err.what();
            log.close();
       }
    }
    return 0;
}
