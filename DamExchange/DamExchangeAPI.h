#pragma once
#include <string>

class DXP_AbstractMessage;

class DXP_Socket // public boost::asio::ip::tcp::socket
{
        void connect(const std::string, size_t);
        void close(void);

        void receive(const DXP_AbstractMessage&);       // + handler
        void send(const DXP_AbstractMessage&);          // + handler
};
