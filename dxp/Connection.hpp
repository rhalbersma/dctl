#pragma once
#include <deque>                        // deque
#include <iomanip>
#include <iostream>
#include <string>                       // string
#include <boost/asio.hpp>
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/bind.hpp>
#include <boost/thread.hpp>             // thread
#include "Traits.hpp"
#include "Version.hpp"

namespace dctl {
namespace dxp {

namespace asio = boost::asio;

template<typename Protocol = protocol>
class Connection
{
public:

        // constructors
        Connection()
        :
                socket_(io_service_),
                acceptor_(io_service_)
        {
        }

        // connectors

        // connect to default port and host
        void connect()
        {
                do_connect(asio::ip::tcp::endpoint(LOOPBACK, PORT));
        }

        // connect to user supplied port on default host
        void connect(unsigned short port)
        {
                do_connect(asio::ip::tcp::endpoint(LOOPBACK, port));
        }

        // connect to default port on user supplied host
        void connect(const std::string& host)
        {
                do_connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(host), PORT));
        }


        // connect to user supplied port and host
        void connect(const std::string& host, unsigned short port)
        {
                do_connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(host), port));
        }

        // acceptors
        // accept on default port
        void accept()
        {
                do_accept(asio::ip::tcp::endpoint(PROTOCOL, PORT));
        }

        // accept on user supplied port
        void accept(unsigned short port)
        {
                do_accept(asio::ip::tcp::endpoint(PROTOCOL, port));
        }

        void close()
        {
                // do_close() will only be called in a thread in which io_service::run() is currently being invoked.
                io_service_.post(boost::bind(&Connection<Protocol>::do_close, this));
        }

        void read(std::string& message)
        {
                // do_read() will only be called in a thread in which io_service::run() is currently being invoked.
                io_service_.post(boost::bind(&Connection<Protocol>::do_read, this, message));
        }

        void write(const std::string& message)
        {
                // do_write() will only be called in a thread in which io_service::run() is currently being invoked.
                io_service_.post(boost::bind(&Connection<Protocol>::do_write, this, message));
        }

private:
        // implementation
        void do_connect(const asio::ip::tcp::endpoint& endpoint)
        {
                socket_.async_connect(
                        endpoint,
                        boost::bind(&Connection<Protocol>::handle_open, this, asio::placeholders::error)
                );
                start_event_loop();
        }

        void do_accept(const asio::ip::tcp::endpoint& endpoint)
        {
                acceptor_.open(endpoint.protocol());
                acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
                acceptor_.bind(endpoint);
                acceptor_.listen();
                acceptor_.async_accept(
                        socket_,
                        boost::bind(&Connection<Protocol>::handle_open, this, asio::placeholders::error)
                );
                start_event_loop();
        }

        void handle_open(const boost::system::error_code& error)
        {
                if (!error) {
                        async_read_next();
                }
        }

        void do_close()
        {
                boost::system::error_code ec;
                acceptor_.close(ec);

                socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
                socket_.close(ec);

                io_service_.stop();
                stop_event_loop();
        }

        void start_event_loop()
        {
                io_service_thread_ = boost::thread((boost::bind(&asio::io_service::run, &io_service_)));
        }

        void stop_event_loop()
        {
                io_service_thread_.join();
        }

        void async_read_next()
        {
                asio::async_read_until(
                        socket_,
                        read_buf_,
                        TERMINATOR,
                        boost::bind(&Connection<Protocol>::handle_read, this, asio::placeholders::error)
                );
        }

        void handle_read(const boost::system::error_code& error)
        {
                if (!error) {
                        std::istream read_buf_stream(&read_buf_);
                        std::string message;
                        std::getline(read_buf_stream, message, TERMINATOR);
                        read_messages_.push_back(message);
                        std::cout << message << "\n";
                        async_read_next();
                } else {
                        do_close();
                }
        }

        void do_read(std::string& message)
        {
                message = read_messages_.front();
                read_messages_.pop_front();
        }

        // <message> is passed by-value instead of by-reference, or it might
        // go out of scope before being used in the io_service::run() thread
        void do_write(std::string message)
        {
                bool no_write_in_progress = write_messages_.empty();
                write_messages_.push_back(message);
                if (no_write_in_progress) {
                        async_write_next();
                }
        }

        void async_write_next()
        {
                asio::async_write(
                        socket_,
                        asio::buffer(write_messages_.front().c_str(), write_messages_.front().length() + 1),
                        boost::bind(&Connection<Protocol>::handle_write, this, asio::placeholders::error)
                );
        }

        void handle_write(const boost::system::error_code& error)
        {
                if (!error) {
                        write_messages_.pop_front();
                        if (!write_messages_.empty()) {
                                async_write_next();
                        }
                } else {
                        do_close();
                }
        }

        // representation
        BOOST_STATIC_CONSTANT(auto, PROTOCOL = asio::ip::tcp::v4());
        BOOST_STATIC_CONSTANT(auto, LOOPBACK = asio::ip::address_v4::loopback());
        BOOST_STATIC_CONSTANT(unsigned short, PORT = port<Protocol>::value);
        BOOST_STATIC_CONSTANT(auto, TERMINATOR = terminator<Protocol>::value);

        asio::io_service io_service_;
        asio::ip::tcp::acceptor acceptor_;
        asio::ip::tcp::socket socket_;

        boost::thread io_service_thread_;
        asio::streambuf read_buf_;

        typedef std::deque<std::string> MessageQueue;
        MessageQueue read_messages_;
        MessageQueue write_messages_;
};

}       // namespace dxp
}       // namespace dctl
