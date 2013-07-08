#pragma once
#include <string>                                       // string
#include <boost/assert.hpp>                             // BOOST_ASSERT
#include <boost/config.hpp>                             // BOOST_STATIC_CONSTANT
#include <boost/utility.hpp>                            // noncopyable
#include <dctl/factory/header_body_terminator.hpp>      // make_header_body_terminator

namespace dctl {
namespace dxp {

/*

        The format and semantics of Message are defined at:
        http://www.mesander.nl/damexchange/edxplg2.htm

*/

class Message
:
        public factory::make_header_body_terminator< 1, 126, '\0' >,
        private boost::noncopyable
{
public:
        // enable deletion of a Derived* through a Base*
        virtual ~Message() = default;

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
                        static_cast<int>(do_header().length()) == header_length_ &&
                        static_cast<int>(do_body().length()) <= max_body_length_
                );
        }

        // pure virtual implementation
        virtual std::string do_header() const = 0;
        virtual std::string do_body() const = 0;
};

}       // namespace dxp
}       // namespace dctl
