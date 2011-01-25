#include "Client.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace DXP = DamExchangeProtocol;

DXP::Client::Client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator)
: 
        io_service_(io_service),
        socket_(io_service)
{
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect
        (
                endpoint,
                boost::bind(&Client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator)
        );
}

void DXP::Client::handle_connect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator)
{
        if (!error) {
                boost::asio::async_read
                (
                        socket_,
                        boost::asio::buffer(read_msg_.data(), chat_message::HEADER_LENGTH),
                        boost::bind(&Client::handle_read_header, this, boost::asio::placeholders::error)
                );
        } else if (endpoint_iterator != tcp::resolver::iterator()) {
                socket_.close();
                tcp::endpoint endpoint = *endpoint_iterator;
                socket_.async_connect
                (
                        endpoint,
                        boost::bind(&Client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator)
                );
        }
}

void DXP::Client::handle_read_header(const boost::system::error_code& error)
{
        if (!error && read_msg_.decode_header()){
                boost::asio::async_read
                (
                        socket_,
                        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                        boost::bind(&Client::handle_read_body, this, boost::asio::placeholders::error)
                );
        } else {
                do_close();
        }
}

void DXP::Client::handle_read_body(const boost::system::error_code& error)
{
        if (!error) {
                std::cout.write(read_msg_.body(), read_msg_.body_length());
                std::cout << "\n";
                boost::asio::async_read
                (
                        socket_,
                        boost::asio::buffer(read_msg_.data(), chat_message::HEADER_LENGTH),
                        boost::bind(&Client::handle_read_header, this, boost::asio::placeholders::error)
                );
        } else {
                do_close();
        }
}

void DXP::Client::write(const chat_message& msg)
{
        io_service_.post(boost::bind(&Client::do_write, this, msg));
}

void DXP::Client::do_write(const chat_message& msg)
{
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
                boost::asio::async_write
                (
                        socket_,
                        boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                        boost::bind(&Client::handle_write, this, boost::asio::placeholders::error)
                );
        }
}

void DXP::Client::handle_write(const boost::system::error_code& error)
{
        if (!error) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) {
                        boost::asio::async_write
                        (
                                socket_,
                                boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                                boost::bind(&Client::handle_write, this, boost::asio::placeholders::error)
                        );
                }
        } else {
                do_close();
        }
}

void DXP::Client::close(void)
{
        io_service_.post(boost::bind(&Client::do_close, this));
}

void DXP::Client::do_close(void)
{
        socket_.close();
}
