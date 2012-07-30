#pragma once
#include <deque>                        // deque
#include <iomanip>
#include <iostream>
#include <string>                       // string
#include <boost/asio.hpp>
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/bind.hpp>               // bind
#include <boost/thread.hpp>             // thread
#include "ConnectionInterface.hpp"

namespace dctl {
namespace dxp {

class Acceptor
:
        public ConnectionInterface
{
public:

        // structors
        Acceptor()
        :
                socket_(io_service_),
                acceptor_(io_service_)
        {
        }

        // accept on user supplied port
        void accept(unsigned short port)
        {
                do_accept(boost::asio::ip::tcp::endpoint(PROTOCOL, port));
        }

private:
        // accept on default port
        virtual void do_open()
        {
                do_accept(boost::asio::ip::tcp::endpoint(PROTOCOL, port_));
        }

        void do_accept(const boost::asio::ip::tcp::endpoint& endpoint)
        {
                acceptor_.open(endpoint.protocol());
                acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
                acceptor_.bind(endpoint);
                acceptor_.listen();
                acceptor_.async_accept(
                        socket_,
                        boost::bind(&Acceptor::handle_open, this, boost::asio::placeholders::error)
                );
                start_event_loop();
        }

        void handle_open(const boost::system::error_code& error)
        {
                if (!error) {
                        async_read_next();
                }
        }

        virtual void do_close()
        {
                // post_close() will only be called in a thread in which io_service()() is currently being invoked.
                io_service_.post(boost::bind(&Acceptor::post_close, this));
        }

        void post_close()
        {
                boost::system::error_code ec;
                acceptor_.close(ec);

                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                socket_.close(ec);

                io_service_.stop();
                stop_event_loop();
        }

        void start_event_loop()
        {
                io_service_thread_ = boost::thread((boost::bind(&boost::asio::io_service(), &io_service_)));
        }

        void stop_event_loop()
        {
                io_service_thread_.join();
        }

        // representation
        BOOST_STATIC_CONSTANT(auto, PROTOCOL = boost::asio::ip::tcp::v4());
        BOOST_STATIC_CONSTANT(auto, LOOPBACK = boost::asio::ip::address_v4::loopback());

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
