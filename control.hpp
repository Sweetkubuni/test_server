#ifndef CONTROL_HPP
#define CONTROL_HPP
#include "threadsafe_queue.hpp"
class control
{
    protected:
	threadsafe::queue< std::string > out_mq;
	threadsafe::queue< std::string > in_mq;
	unique_ptr<std::thread> th;
	bool keep_alive;
	void run();
	public:
        control();
        ~control();
        shared_ptr<std::string> receive();
        void send(const std::string & msg);
};
#endif // CONTROL_HPP


