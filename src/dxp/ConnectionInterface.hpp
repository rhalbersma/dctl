#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace dxp {

class ConnectionInterface
{
public:
        // virtual destructor
        virtual ~ConnectionInterface()
        {
                // no-op
        }

        // non-virtual interface
        void open()
        {
                do_open();
        }

        void close()
        {
                do_close();
        }

protected:
        // representation
        BOOST_STATIC_CONSTANT(unsigned short, port_ = 27531);

private:
        // pure virtual implementation
        virtual void do_open() = 0;
        virtual void do_close() = 0;
};

}       // namespace dxp
}       // namespace dctl
