#include <iomanip>
#include <iostream>
#include <boost/bind.hpp>
#include "Session.h"
#include "Scanner.h"

namespace damexchange {
namespace layer1 {

const boost::asio::ip::tcp Session::PROTOCOL = boost::asio::ip::tcp::v4();
const boost::asio::ip::address_v4 Session::LOOPBACK = boost::asio::ip::address_v4::loopback();

Session::Session()
:
        socket_(io_service_),
        acceptor_(io_service_)
{
}

void Session::connect()
{
        do_connect(boost::asio::ip::tcp::endpoint(LOOPBACK, PORT));
}

void Session::connect(unsigned short port)
{
        do_connect(boost::asio::ip::tcp::endpoint(LOOPBACK, port));
}

void Session::connect(const std::string& host)
{
        do_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), PORT));
}

void Session::connect(const std::string& host, unsigned short port)
{
        do_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port));
}

void Session::accept()
{
        do_accept(boost::asio::ip::tcp::endpoint(PROTOCOL, PORT));
}

void Session::accept(unsigned short port)
{
        do_accept(boost::asio::ip::tcp::endpoint(PROTOCOL, port));
}

void Session::do_connect(const boost::asio::ip::tcp::endpoint& endpoint)
{
        socket_.async_connect(
                endpoint,
                boost::bind(&Session::handle_open, this, boost::asio::placeholders::error)
        );
        start_event_loop();
}

void Session::do_accept(const boost::asio::ip::tcp::endpoint& endpoint)
{
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
        acceptor_.async_accept(
                socket_,
                boost::bind(&Session::handle_open, this, boost::asio::placeholders::error)
        );
        start_event_loop();
}

void Session::start_event_loop()
{
        io_service_thread_ = boost::thread((boost::bind(&boost::asio::io_service::run, &io_service_)));
}

void Session::stop_event_loop()
{
        io_service_thread_.join();
}

void Session::handle_open(const boost::system::error_code& error)
{
        if (!error) {
                async_read_next();
        } 
}

void Session::close()
{
        // do_close() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Session::do_close, this));
}

void Session::do_close()
{
        socket_.close();
        stop_event_loop();
}

void Session::async_read_next()
{
        boost::asio::async_read_until(
                socket_,
                read_buf_,
                Scanner::terminator(),
                boost::bind(&Session::handle_read, this, boost::asio::placeholders::error)
        );
}

void Session::handle_read(const boost::system::error_code& error)
{
        if (!error) {
                std::istream read_buf_stream(&read_buf_);
                std::string msg;
                std::getline(read_buf_stream, msg, Scanner::terminator());
                read_msgs_.push_back(msg);
                std::cout << msg << std::endl;
                async_read_next();
        } else {
                do_close();
        }
}

void Session::read(std::string& msg)
{
        // do_read() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Session::do_read, this, msg));
}

void Session::do_read(std::string& msg)
{
        msg = read_msgs_.front();
        read_msgs_.pop_front();
}

void Session::write(const std::string& msg)
{
        // do_write() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Session::do_write, this, msg));
}

// by-value instead of by-reference, or <msg> might go out of scope before being used in the io_service::run() thread
void Session::do_write(std::string msg)
{
        bool no_write_in_progress = write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (no_write_in_progress) {
                async_write_next();
        }
}

void Session::async_write_next()
{
        boost::asio::async_write(
                socket_,
                boost::asio::buffer(write_msgs_.front().c_str(), write_msgs_.front().length() + 1),
                boost::bind(&Session::handle_write, this, boost::asio::placeholders::error)
        );
}

void Session::handle_write(const boost::system::error_code& error)
{
        if (!error) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) {
                        async_write_next();
                }
        } else {
                do_close();
        }
}

}       // namespace layer1
}       // namespace damexchange
