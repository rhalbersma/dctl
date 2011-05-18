#pragma once
#include <string>

namespace DamExchange {
namespace Layer1 {

class Scanner
{
public:
        // constructors
        explicit Scanner(const std::string&);

        // views
        std::string str(void) const;
        std::string header(void) const;
        std::string body(void) const;
        const char* c_str(void) const;
        size_t length(void) const;
        static char terminator(void);

private:
        // invariants
        bool invariant(void) const;

        // implementation
        static const size_t HEADER_LENGTH = 1;
        static const size_t MAX_BODY_LENGTH = 126;
        static const char TERMINATOR = '\0';

        // representation
        std::string message_;
};

}       // namespace Layer1
}       // namespace DamExchange
