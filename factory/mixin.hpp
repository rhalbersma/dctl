#pragma once
#include <memory>                       // unique_ptr
#include <string>                       // string
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
public:
        static std::string header(const std::string& input)
        {
                return input.substr(0, HeaderLength);
        }

        static std::string body(const std::string& input)
        {
                return input.substr(HeaderLength);
        }

protected:
        BOOST_STATIC_CONSTANT(auto, header_length_ = HeaderLength);
        BOOST_STATIC_CONSTANT(auto, max_body_length_ = MaxBodyLength);
};

template
<
        char Identifier,
        typename ConcreteProduct,
        typename AbstractProduct
>
struct IdentifierCreateObject
{
public:
        static std::unique_ptr<AbstractProduct> create_object(const std::string& parameter)
        {
                return std::unique_ptr<ConcreteProduct>(new ConcreteProduct(parameter));
        }

        static std::string identifier()
        {
                return std::string(1, identifier_);
        }

protected:
        BOOST_STATIC_CONSTANT(auto, identifier_ = Identifier);
};

}       // namespace mixin
}       // namespace dctl
