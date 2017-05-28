#pragma once
#include <cstddef>      // size_t
#include <string>       // string
#include <type_traits>  // is_base_of

namespace dctl {
namespace factory {

template
<
        std::size_t HeaderLength,
        std::size_t MaxBodyLength,
        char Terminator
>
struct make_header_body_terminator
{
        static auto header(std::string const& input)
        {
                return input.substr(0, HeaderLength);
        }

        static auto body(std::string const& input)
        {
                return input.substr(HeaderLength);
        }

        static auto terminator()
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
