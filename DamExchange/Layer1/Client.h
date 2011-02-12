#pragma once
#include <deque>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

namespace DamExchange {
namespace Layer1 {

class Client
{
public:
        // constructors
        Client(void);
        ~Client(void);

        // interface
        void connect(const std::string&, const std::string&);
        void accept(const std::string&);
        std::string read(void);
        void write(const std::string&);
        void close(void);

private:
        // implementation
        void async_connect_next(tcp::resolver::iterator);
        void handle_connect(const boost::system::error_code&, tcp::resolver::iterator);

        void handle_accept(const boost::system::error_code&);

        void async_read_next(void);
        void handle_read(const boost::system::error_code&);

        void do_write(std::string);
        void async_write_next(void);
        void handle_write(const boost::system::error_code&);

        void do_close(void);

        // representation        
        boost::asio::io_service io_service_;
        tcp::acceptor acceptor_;
        tcp::socket socket_;         
        boost::thread read_thread_;
        boost::asio::streambuf incoming_;

        typedef std::deque<std::string> MessageQueue;
        MessageQueue read_msgs_;
        MessageQueue write_msgs_;
};

}       // namespace Layer1
}       // namespace DamExchange
