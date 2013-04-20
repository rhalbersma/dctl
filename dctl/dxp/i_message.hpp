#pragma once
#include <string>                               // string
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/config.hpp>                     // BOOST_STATIC_CONSTANT
#include <boost/utility.hpp>                    // noncopyable
#include <dctl/factory/mixin.hpp>               // HeaderBodyTerminator
#include <dctl/preprocessor/cpp11/default.hpp>  // DCTL_PP_IS_DEFAULT

namespace dctl {
namespace dxp {

/*

        The format and semantics of IMessage are defined at:
        http://www.mesander.nl/damexchange/edxplg2.htm

*/

class IMessage
:
        public mixin::HeaderBodyTerminator< 1, 126, '\0' >,
        private boost::noncopyable
{
public:
        // enable deletion of a Derived* through a Base*
        virtual ~IMessage() DCTL_PP_IS_DEFAULT

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
