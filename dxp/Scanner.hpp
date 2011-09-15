#pragma once
#include <string>                       // std::string
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Traits.hpp"

namespace dctl {
namespace dxp {

template<typename Protocol>
class Scanner
{
public:
        // constructors
        explicit Scanner(const std::string& message)
        :
                message_(message)
        {
                BOOST_ASSERT(invariant());
        }

        // views
        std::string header() const
        {
                return message_.substr(0, HEADER_LENGTH);
        }

        std::string body() const
        {
                return message_.substr(HEADER_LENGTH);
        }

private:
        // invariants
        bool invariant() const
        {
                return (
                        header().length() == HEADER_LENGTH &&
                        body().length() <= MAX_BODY_LENGTH
                );
        }

        // implementation
        BOOST_STATIC_CONSTANT(auto, HEADER_LENGTH = header_length<Protocol>::value);
        BOOST_STATIC_CONSTANT(auto, MAX_BODY_LENGTH = max_body_length<Protocol>::value);

        // representation
        std::string message_;
};

}       // namespace dxp
}       // namespace dctl
