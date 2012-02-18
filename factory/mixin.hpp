#pragma once
#include <memory>                       // unique_ptr
#include <string>                       // string
#include <type_traits>                  // is_base_of
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace mixin {

template
<
        size_t HeaderLength,
        size_t MaxBodyLength
>
struct HeaderBody
{
        static std::string header(const std::string& input)
        {
                return input.substr(0, HeaderLength);
        }

        static std::string body(const std::string& input)
        {
                return input.substr(HeaderLength);
        }

        BOOST_STATIC_CONSTANT(auto, header_length_ = HeaderLength);
        BOOST_STATIC_CONSTANT(auto, max_body_length_ = MaxBodyLength);
};

template
<
        char Identifier,
        typename Derived,
        typename Base
>
struct IdentifierCreate
{
        static std::unique_ptr<Base> create(const std::string& parameter)
        {
                return std::unique_ptr<Base>(new Derived(parameter));
        }

        static std::string identifier()
        {
                return std::string(1, identifier_);
        }

        typedef Base base;
        BOOST_STATIC_CONSTANT(auto, identifier_ = Identifier);
};

template<typename T>
struct has_header_body
:
        std::is_base_of< HeaderBody< T::header_length_, T::max_body_length_ >, T >
{};

template<typename T>
struct has_identifier_create
:
        std::is_base_of< IdentifierCreate< T::identifier_, T, typename T::base >, T >
{};

}       // namespace mixin
}       // namespace dctl
