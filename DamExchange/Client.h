#pragma once
#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "StringMessage.h"

using boost::asio::ip::tcp;

namespace DamExchangeProtocol {

class Client
{
public:
        // constructors
        Client(const std::string&, const std::string&);
        ~Client(void);

        // interface
        void write(const StringMessage&);
        void close(void);

private:
        // implementation
        void async_connect_next(tcp::resolver::iterator);
        void handle_connect(const boost::system::error_code&, tcp::resolver::iterator);

        void async_read_next(void);
        void handle_read(const boost::system::error_code&);

        void do_write(StringMessage);
        void async_write_next(void);
        void handle_write(const boost::system::error_code&);

        void do_close(void);

        // representation        
        boost::asio::io_service io_service_;
        tcp::socket socket_;         
        boost::asio::streambuf incoming_;
        boost::thread read_thread_;

        typedef std::deque<StringMessage> StringMessageQueue;
        StringMessageQueue read_msgs_;
        StringMessageQueue write_msgs_;
};

}       // namespace DamExchangeProtocol
