#pragma once
#include <string>                       // string
#include <type_traits>                  // is_base_of
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace factory {

template
<
        int HeaderLength,
        int MaxBodyLength,
        char Terminator
>
struct make_header_body_terminator
{
        static std::string header(std::string const& input)
        {
                return input.substr(0, HeaderLength);
        }

        static std::string body(std::string const& input)
        {
                return input.substr(HeaderLength);
        }

        static char terminator()
        {
                return terminator_;
        }

        BOOST_STATIC_CONSTANT(auto, header_length_ = HeaderLength);
        BOOST_STATIC_CONSTANT(auto, max_body_length_ = MaxBodyLength);
        BOOST_STATIC_CONSTANT(auto, terminator_ = Terminator);
};

template<class T>
struct has_header_body_terminator
:
        std::is_base_of<
                make_header_body_terminator<
                        T::header_length_,
                        T::max_body_length_,
                        T::terminator_
                >,
                T
        >
{};

}       // namespace factory
}       // namespace dctl
