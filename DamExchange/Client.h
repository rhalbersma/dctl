#pragma once
#include "AbstractMessage.h"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

namespace DamExchangeProtocol {

class Client
{
public:
        // typedefs
        typedef std::shared_ptr<AbstractMessage> chat_message;                

        // constructors
        Client(boost::asio::io_service&, tcp::resolver::iterator);

        // interface
        void write(chat_message);
        void close(void);

private:
        // implementation
        void handle_connect(const boost::system::error_code&, tcp::resolver::iterator);
        void handle_read(const boost::system::error_code&);
        void do_write(chat_message);
        void handle_write(const boost::system::error_code&);
        void do_close(void);

        // representation
        boost::asio::io_service& io_service_;
        tcp::socket socket_;
        
        chat_message read_msg_;
        typedef std::deque<chat_message> chat_message_queue;
        chat_message_queue write_msgs_;
};

}       // namespace DamExchangeProtocol
