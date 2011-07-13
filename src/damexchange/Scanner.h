#pragma once
#include <string>

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

        // representation
        std::string message_;
};

}       // namespace damexchange

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Scanner.hpp"
