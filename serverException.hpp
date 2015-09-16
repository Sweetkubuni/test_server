#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP
#include <exception>

struct ServerException : public exception
{
    std::string msg;

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
