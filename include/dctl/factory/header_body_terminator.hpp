#pragma once
#include <string>                       // string
#include <type_traits>                  // is_base_of

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

        static const auto header_length_ = HeaderLength;
        static const auto max_body_length_ = MaxBodyLength;
        static const auto terminator_ = Terminator;
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
