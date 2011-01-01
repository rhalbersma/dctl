#pragma once
#include <string>

class DXP_StringMessage
{
public:
        // constructors
        explicit DXP_StringMessage(const std::string&);
        DXP_StringMessage(const std::string&, const std::string&);

        // views
        const char* c_str(void) const;
        std::string str(void) const;
        std::string header(void) const;
        std::string body(void) const;

private:
        // invariants
        bool invariant(void) const;
        bool invariant(const std::string&, const std::string&) const;

        // implementation
        static const size_t HEADER_LENGTH = 1;
        static const size_t MAX_BODY_LENGTH = 126;

        // representation
        std::string content_;
};
