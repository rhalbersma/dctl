#include "Client.h"
#include "Scanner.h"
#include <iostream>
#include <boost/bind.hpp>

namespace DXP = DamExchange;

DXP::Layer1::Client::Client(void)
:
        acceptor_(io_service_),
        socket_(io_service_)
{
}

DXP::Layer1::Client::~Client(void)
{
        do_close();
        read_thread_.join();
}

void DXP::Layer1::Client::connect(const std::string& host_name, const std::string& service_name)
{
        tcp::resolver::query query(host_name, service_name);
        tcp::resolver resolver(io_service_);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        async_connect_next(endpoint_iterator);
        read_thread_ = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
}

void DXP::Layer1::Client::async_connect_next(tcp::resolver::iterator endpoint_iterator)
{
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(
                endpoint,
                boost::bind(&Client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator)
        );
}

void DXP::Layer1::Client::handle_connect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator)
{
        if (!error) {
                async_read_next();
        } else if (endpoint_iterator != tcp::resolver::iterator()) {
                do_close();
                async_connect_next(endpoint_iterator);
        }
}

void DXP::Layer1::Client::accept(const std::string& service_name)
{
        tcp::endpoint endpoint(tcp::v4(), atoi(service_name.c_str()));
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        acceptor_.async_accept(
                socket_,
                boost::bind(&Client::handle_accept, this, boost::asio::placeholders::error)
        );
        read_thread_ = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
}

void DXP::Layer1::Client::handle_accept(const boost::system::error_code& error)
{
        if (!error) {
                async_read_next();
        }
}

std::string DXP::Layer1::Client::read(void)
{
        std::string msg = read_msgs_.front();
        read_msgs_.pop_front();
        return msg;
}

void DXP::Layer1::Client::async_read_next(void)
{
        boost::asio::async_read_until(
                socket_,
                incoming_,
                Scanner::terminator(),
                boost::bind(&Client::handle_read, this, boost::asio::placeholders::error)
        );
}

void DXP::Layer1::Client::handle_read(const boost::system::error_code& error)
{
        if (!error) {
                std::istream incoming_stream(&incoming_);
                std::string msg;
                std::getline(incoming_stream, msg, Scanner::terminator());
                read_msgs_.push_back(msg);
                std::cout << msg << std::endl;
                async_read_next();
        } else {
                do_close();
        }
}

void DXP::Layer1::Client::write(const std::string& msg)
{
        // do_write() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Client::do_write, this, msg));
}

// by-value instead of by-reference, or <msg> might go out of scope before being used in the io_service::run() thread
void DXP::Layer1::Client::do_write(std::string msg)
{
        bool no_write_in_progress = write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (no_write_in_progress) {
                async_write_next();
        }
}

void DXP::Layer1::Client::async_write_next(void)
{
        boost::asio::async_write(
                socket_,
                boost::asio::buffer(write_msgs_.front().c_str(), write_msgs_.front().length() + 1),
                boost::bind(&Client::handle_write, this, boost::asio::placeholders::error)
        );
}

void DXP::Layer1::Client::handle_write(const boost::system::error_code& error)
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

void DXP::Layer1::Client::close(void)
{
        // do_close() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Client::do_close, this));
}

void DXP::Layer1::Client::do_close(void)
{
        socket_.close();
}
