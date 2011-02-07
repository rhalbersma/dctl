#include "Client.h"
#include <iostream>
#include <boost/bind.hpp>

namespace DXP = DamExchangeProtocol;

DXP::Client::Client(const std::string& host_name, const std::string& service_name)
: 
        socket_(io_service_)
{
        tcp::resolver::query query(host_name, service_name);
        tcp::resolver resolver(io_service_);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        async_connect_next(endpoint_iterator);
        read_thread_ = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
}

DXP::Client::~Client(void)
{
        do_close();
        read_thread_.join();
}

void DXP::Client::async_connect_next(tcp::resolver::iterator endpoint_iterator)
{
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect (
                endpoint,
                boost::bind(&Client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator)
        );
}

void DXP::Client::handle_connect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator)
{
        if (!error) {
                async_read_next();
        } else if (endpoint_iterator != tcp::resolver::iterator()) {
                do_close();
                async_connect_next(endpoint_iterator);
        }
}

void DXP::Client::async_read_next(void)
{
        boost::asio::async_read_until (
                socket_,
                incoming_,
                StringMessage::terminator(),
                boost::bind(&Client::handle_read, this, boost::asio::placeholders::error)
        );
}

void DXP::Client::handle_read(const boost::system::error_code& error)
{
        if (!error) {
                std::istream incoming_stream(&incoming_);
                std::string msg;
                std::getline(incoming_stream, msg, StringMessage::terminator());
                read_msgs_.push_back(StringMessage(msg));
                std::cout << msg << std::endl;
                async_read_next();
        } else {
                do_close();
        }
}

void DXP::Client::write(const StringMessage& msg)
{
        // do_write() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Client::do_write, this, msg));
}

// by-value instead of by-reference, or <msg> might go out of scope before being used in the io_service::run() thread
void DXP::Client::do_write(StringMessage msg)
{
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
                async_write_next();
        }
}

void DXP::Client::async_write_next(void)
{
        boost::asio::async_write (
                socket_,
                boost::asio::buffer(write_msgs_.front().c_str(), write_msgs_.front().length() + 1),
                boost::bind(&Client::handle_write, this, boost::asio::placeholders::error)
        );
}

void DXP::Client::handle_write(const boost::system::error_code& error)
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

void DXP::Client::close(void)
{
        // do_close() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Client::do_close, this));
}

void DXP::Client::do_close(void)
{
        socket_.close();
}
