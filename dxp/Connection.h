#pragma once
#include <deque>        // std::deque
#include <string>       // std::string
#include <boost/asio.hpp>
#include <boost/config.hpp>
#include "Protocol.hpp"

namespace dctl {
namespace dxp {

template<typename Protocol>
class Connection
{
public:
        // constructors
        Connection();

        // connectors
        void connect();                                         // connect to default port and host
        void connect(unsigned short);                           // connect to user supplied port on default host
        void connect(const std::string&);                       // connect to default port on user supplied host
        void connect(const std::string&, unsigned short);       // connect to user supplied port and host

        // acceptors
        void accept();                                          // accept on default port                           
        void accept(unsigned short);                            // accept on user supplied port

        void close();

        void read(std::string&);
        void write(const std::string&);

private:
        // implementation
        void do_connect(const boost::asio::ip::tcp::endpoint&);
        void do_accept(const boost::asio::ip::tcp::endpoint&);
        void handle_open(const boost::system::error_code&);

        void do_close();
        void handle_close();

        void start_event_loop();
        void stop_event_loop();

        void async_read_next();
        void handle_read(const boost::system::error_code&);
        void do_read(std::string&);

        void do_write(std::string);
        void async_write_next();
        void handle_write(const boost::system::error_code&);

        // representation
        BOOST_STATIC_CONSTANT(auto, PROTOCOL = boost::asio::ip::tcp::v4());
        BOOST_STATIC_CONSTANT(auto, LOOPBACK = boost::asio::ip::address_v4::loopback());
        BOOST_STATIC_CONSTANT(unsigned short, PORT = port<Protocol>::value);
        BOOST_STATIC_CONSTANT(auto, TERMINATOR = terminator<Protocol>::value);

        boost::asio::io_service io_service_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::ip::tcp::socket socket_;

        boost::thread io_service_thread_;
        boost::asio::streambuf read_buf_;

        typedef std::deque<std::string> MessageQueue;
        MessageQueue read_messages_;
        MessageQueue write_messages_;
};

}       // namespace dxp
}       // namespace dctl

// include template definitions inside header
#include "Connection.hpp"
