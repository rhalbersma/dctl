#include "Server.h"
#include "Scanner.h"
#include <iostream>
#include <boost/bind.hpp>

namespace DXP = DamExchange;

DXP::Layer1::Server::Server(void)
:
        endpoint_(tcp::endpoint(tcp::v4(), 27531)),
        acceptor_(io_service_),
        socket_(io_service_)
{
}

void DXP::Layer1::Server::do_open(void)
{
        acceptor_.async_accept(
                socket_,
                boost::bind(&Server::handle_open, this, boost::asio::placeholders::error)
        );
        //read_thread_ = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
}

void DXP::Layer1::Server::handle_open(const boost::system::error_code& error)
{
        if (!error) {
                communication_ = std::make_shared<Communication>(this);
        }
}

void DXP::Layer1::Server::do_close(void)
{
        // handle_close() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Server::handle_close, this));
}

void DXP::Layer1::Server::handle_close(void)
{
        acceptor_.close();
}

tcp::socket& DXP::Layer1::Server::get_socket(void)
{
        return socket_;
}

void DXP::Layer1::Server::do_read(std::string& msg)
{
        communication_->read(msg);
}

void DXP::Layer1::Server::do_write(const std::string& msg)
{
        // handle_write() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Server::handle_write, this, msg));
}

// by-value instead of by-reference, or <msg> might go out of scope before being used in the io_service::run() thread
void DXP::Layer1::Server::handle_write(std::string msg)
{
        communication_->write(msg);
}
