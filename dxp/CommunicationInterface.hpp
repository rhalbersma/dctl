#pragma once
#include <string>                       // std::string
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace dxp {

class CommunicationInterface
{
public:
        // virtual destructor
        virtual ~CommunicationInterface() 
        {
                /* no-op */
        }

        // non-virtual interface
        void read(std::string& message)
        {
                do_read(message);
        }

        void write(const std::string& message)
        {
                do_write(message);
        }

private:
        // pure virtual implementation
        virtual void do_read(std::string&) = 0;
        virtual void do_write(const std::string&) = 0;

        // representation
        BOOST_STATIC_CONSTANT(auto, terminator_ = '\0');
};

}       // namespace dxp
}       // namespace dctl
