#pragma once
#include <string>
#include "DamExchange.h"

namespace dctl {
namespace protocol {
namespace damexchange {

template<typename Protocol>
class Scanner
{
public:
        // constructors
        explicit Scanner(const std::string&);

        // views
        std::string header() const;
        std::string body() const;

private:
        // invariants
        bool invariant() const;

        // implementation
        static const int HEADER_LENGTH = header_length<Protocol>::value;
        static const int MAX_BODY_LENGTH = max_body_length<Protocol>::value;

        // representation
        std::string message_;
};

}       // namespace damexchange
}       // namespace protocol
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Scanner.hpp"
