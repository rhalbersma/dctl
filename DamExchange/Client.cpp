#include "Client.h"
#include <iostream>
#include <boost/bind.hpp>

namespace DXP = DamExchangeProtocol;

DXP::Client::Client(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
: 
        io_service_(io_service),
        socket_(io_service)
{
        tcp::resolver resolver(io_service_);
        tcp::resolver::query query(host, port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        async_connect_next(endpoint_iterator);
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
                chat_message::terminator(),
                boost::bind(&Client::handle_read, this, boost::asio::placeholders::error)
        );
}

void DXP::Client::handle_read(const boost::system::error_code& error)
{
        if (!error) {
                std::istream incoming_stream(&incoming_);
                std::string msg;
                std::getline(incoming_stream, msg, chat_message::terminator());
                read_msgs_.push_back(chat_message(msg));
                std::cout << msg << std::endl;
                async_read_next();
        } else {
                do_close();
        }
}

void DXP::Client::write(const chat_message& msg)
{
        // do_write() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Client::do_write, this, msg));
}

void DXP::Client::do_write(const chat_message& msg)
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
                boost::asio::buffer(write_msgs_.front().str()),
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
