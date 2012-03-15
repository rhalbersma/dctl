#pragma once
#include <string>                       // string
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "../factory/mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The format and semantics of MessageInterface are defined at:
        http://www.mesander.nl/damexchange/edxplg2.htm

*/

class MessageInterface
:
        public mixin::HeaderBodyTerminator< 1, 126, '\0' >
{
public:
        // virtual destructor
        virtual ~MessageInterface()
        {
                // no-op
        }

        // non-virtual interface
        std::string str() const
        {
                BOOST_ASSERT(invariant());
                return do_header() + do_body();
        }

private:
        bool invariant() const
        {
                return (
                        do_header().length() == header_length_ &&
                        do_body().length() <= max_body_length_
                );
        }

        // pure virtual implementation
        virtual std::string do_header() const = 0;
        virtual std::string do_body() const = 0;
};

}       // namespace dxp
}       // namespace dctl
