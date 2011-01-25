#pragma once
#include <deque>
#include <boost/asio.hpp>
#include "StringMessage.h"

using boost::asio::ip::tcp;

namespace DamExchangeProtocol {

typedef StringMessage chat_message; 
typedef std::deque<chat_message> chat_message_queue;

class Client
{
public:
        // constructors
        Client(boost::asio::io_service&, tcp::resolver::iterator);

        // interface
        void write(const chat_message&);
        void close(void);

private:
        // implementation
        void handle_connect(const boost::system::error_code&, tcp::resolver::iterator);
        void handle_read_header(const boost::system::error_code&);
        void handle_read_body(const boost::system::error_code&);

        void do_write(const chat_message&);
        void handle_write(const boost::system::error_code&);

        void do_close(void);

        // representation        
        boost::asio::io_service& io_service_;
        tcp::socket socket_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;
};

}       // namespace DamExchangeProtocol
