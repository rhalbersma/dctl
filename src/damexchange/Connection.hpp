#include <iomanip>
#include <iostream>
#include <boost/bind.hpp>

namespace damexchange {

template<typename Protocol>
Connection<Protocol>::Connection(unsigned short port, char terminator)
:
        socket_(io_service_),
        acceptor_(io_service_)
{
}

template<typename Protocol>
void Connection<Protocol>::connect()
{
        do_connect(boost::asio::ip::tcp::endpoint(LOOPBACK, PORT));
}

template<typename Protocol>
void Connection<Protocol>::connect(unsigned short port)
{
        do_connect(boost::asio::ip::tcp::endpoint(LOOPBACK, port));
}

template<typename Protocol>
void Connection<Protocol>::connect(const std::string& host)
{
        do_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), PORT));
}

template<typename Protocol>
void Connection<Protocol>::connect(const std::string& host, unsigned short port)
{
        do_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port));
}

template<typename Protocol>
void Connection<Protocol>::accept()
{
        do_accept(boost::asio::ip::tcp::endpoint(PROTOCOL, PORT));
}

template<typename Protocol>
void Connection<Protocol>::accept(unsigned short port)
{
        do_accept(boost::asio::ip::tcp::endpoint(PROTOCOL, port));
}

template<typename Protocol>
void Connection<Protocol>::do_connect(const boost::asio::ip::tcp::endpoint& endpoint)
{
        socket_.async_connect(
                endpoint,
                boost::bind(&Connection<Protocol>::handle_open, this, boost::asio::placeholders::error)
        );
        start_event_loop();
}

template<typename Protocol>
void Connection<Protocol>::do_accept(const boost::asio::ip::tcp::endpoint& endpoint)
{
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
        acceptor_.async_accept(
                socket_,
                boost::bind(&Connection<Protocol>::handle_open, this, boost::asio::placeholders::error)
        );
        start_event_loop();
}

template<typename Protocol>
void Connection<Protocol>::start_event_loop()
{
        io_service_thread_ = boost::thread((boost::bind(&boost::asio::io_service::run, &io_service_)));
}

template<typename Protocol>
void Connection<Protocol>::stop_event_loop()
{
        io_service_thread_.join();
}

template<typename Protocol>
void Connection<Protocol>::handle_open(const boost::system::error_code& error)
{
        if (!error) {
                async_read_next();
        } 
}

template<typename Protocol>
void Connection<Protocol>::close()
{
        // do_close() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Connection<Protocol>::do_close, this));
}

template<typename Protocol>
void Connection<Protocol>::do_close()
{
        socket_.close();
        stop_event_loop();
}

template<typename Protocol>
void Connection<Protocol>::async_read_next()
{
        boost::asio::async_read_until(
                socket_,
                read_buf_,
                TERMINATOR,
                boost::bind(&Connection<Protocol>::handle_read, this, boost::asio::placeholders::error)
        );
}

template<typename Protocol>
void Connection<Protocol>::handle_read(const boost::system::error_code& error)
{
        if (!error) {
                std::istream read_buf_stream(&read_buf_);
                std::string msg;
                std::getline(read_buf_stream, msg, TERMINATOR);
                read_msgs_.push_back(msg);
                std::cout << msg << std::endl;
                async_read_next();
        } else {
                do_close();
        }
}

template<typename Protocol>
void Connection<Protocol>::read(std::string& msg)
{
        // do_read() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Connection<Protocol>::do_read, this, msg));
}

template<typename Protocol>
void Connection<Protocol>::do_read(std::string& msg)
{
        msg = read_msgs_.front();
        read_msgs_.pop_front();
}

template<typename Protocol>
void Connection<Protocol>::write(const std::string& msg)
{
        // do_write() will only be called in a thread in which io_service::run() is currently being invoked.
        io_service_.post(boost::bind(&Connection<Protocol>::do_write, this, msg));
}

// by-value instead of by-reference, or <msg> might go out of scope before being used in the io_service::run() thread
template<typename Protocol>
void Connection<Protocol>::do_write(std::string msg)
{
        bool no_write_in_progress = write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (no_write_in_progress) {
                async_write_next();
        }
}

template<typename Protocol>
void Connection<Protocol>::async_write_next()
{
        boost::asio::async_write(
                socket_,
                boost::asio::buffer(write_msgs_.front().c_str(), write_msgs_.front().length() + 1),
                boost::bind(&Connection<Protocol>::handle_write, this, boost::asio::placeholders::error)
        );
}

template<typename Protocol>
void Connection<Protocol>::handle_write(const boost::system::error_code& error)
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

}       // namespace damexchange
