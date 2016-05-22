#pragma once
#include <dctl/utility/factory/header_body_terminator.hpp>      // make_header_body_terminator
#include <cassert>                                      // assert
#include <string>                                       // string

namespace dctl {
namespace dxp {

/*

        The format and semantics of Message are defined at:
        http://www.mesander.nl/damexchange/edxplg2.htm

*/

class Message
:
        public factory::make_header_body_terminator< 1, 126, '\0' >
{
        // pure virtual implementation
        virtual std::string do_header() const = 0;
        virtual std::string do_body() const = 0;

        bool invariant() const
        {
                return
                        do_header().length() == header_length_ &&
                        do_body().length() <= max_body_length_
                ;
        }

public:
        // enable deletion of a Derived* through a Base*
        virtual ~Message() = default;

        // non-virtual interface
        std::string str() const
        {
                assert(invariant());
                return do_header() + do_body();
        }
};

}       // namespace dxp
}       // namespace dctl
