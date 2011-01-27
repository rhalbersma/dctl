#pragma once
#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "StringMessage.h"

using boost::asio::ip::tcp;

namespace DamExchangeProtocol {

typedef StringMessage chat_message; 
typedef std::deque<chat_message> chat_message_queue;

class Client
{
public:
        // constructors
        Client(const std::string&, const std::string&);
        ~Client(void);

        // interface
        void write(const chat_message&);
        void close(void);

private:
        // implementation
        void async_connect_next(tcp::resolver::iterator);
        void handle_connect(const boost::system::error_code&, tcp::resolver::iterator);

        void async_read_next(void);
        void handle_read(const boost::system::error_code&);

        void do_write(chat_message);
        void async_write_next(void);
        void handle_write(const boost::system::error_code&);

        void do_close(void);

        // representation        
        boost::asio::io_service io_service_;
        tcp::socket socket_;         
        boost::asio::streambuf incoming_;
        boost::thread read_thread_;

        chat_message_queue read_msgs_;
        chat_message_queue write_msgs_;
};

}       // namespace DamExchangeProtocol
