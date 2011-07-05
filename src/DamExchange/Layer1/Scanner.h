#pragma once
#include <string>

namespace damexchange {
namespace layer1 {

class Scanner
{
public:
        // constructors
        explicit Scanner(const std::string&);

        // views
        std::string str() const;
        std::string header() const;
        std::string body() const;
        const char* c_str() const;
        size_t length() const;
        static char terminator();

private:
        // invariants
        bool invariant() const;

        // implementation
        static const size_t HEADER_LENGTH = 1;
        static const size_t MAX_BODY_LENGTH = 126;
        static const char TERMINATOR = '\0';

        // representation
        std::string message_;
};

}       // namespace layer1
}       // namespace damexchange
