#pragma once
#include <deque>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace DamExchange {
namespace Layer1 {

class Session
{
public:
        // constructors
        Session(void);

        // connectors
        void connect(void);                             // connect to default port and host
        void connect(size_t);                           // connect to user supplied port on default host
        void connect(const std::string&);               // connect to default port on user supplied host
        void connect(const std::string&, size_t);       // connect to user supplied port and host

        // acceptors
        void accept(void);                              // accept on default port                           
        void accept(size_t);                            // accept on user supplied port

        void close(void);

        void read(std::string&);
        void write(const std::string&);

private:
        // implementation
        void do_connect(const boost::asio::ip::tcp::endpoint&);
        void do_accept(const boost::asio::ip::tcp::endpoint&);
        void handle_open(const boost::system::error_code&);

        void do_close(void);
        void handle_close(void);

        void start_event_loop(void);
        void stop_event_loop(void);

        void async_read_next(void);
        void handle_read(const boost::system::error_code&);
        void do_read(std::string&);

        void do_write(std::string);
        void async_write_next(void);
        void handle_write(const boost::system::error_code&);

        // representation
        static const boost::asio::ip::tcp PROTOCOL;
        static const boost::asio::ip::address_v4 LOOPBACK;
        static const size_t PORT = 27531;

        boost::asio::io_service io_service_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::ip::tcp::socket socket_;

        boost::thread io_service_thread_;
        boost::asio::streambuf read_buf_;

        typedef std::deque<std::string> MessageQueue;
        MessageQueue read_msgs_;
        MessageQueue write_msgs_;
};

}       // namespace Layer1
}       // namespace DamExchange
