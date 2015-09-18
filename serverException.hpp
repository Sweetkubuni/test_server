#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP
#include <exception>
#include <string>
class ServerException : public std::exception
{
    std::string msg;

    public:
    ServerException(const std::string & err_msg)
    {
        msg = err_msg;
    }
    const char * what () const throw ()
    {
        return msg.c_str();
    }
};
#endif // SERVEREXCEPTION_HPP
