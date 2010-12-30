#pragma once
#include <string>

class DXP_String
{
public:
        // constructors
        explicit DXP_String(const std::string&);
        DXP_String(char, const std::string&);

        // views
        std::string str(void) const;
        char header(void) const;
        std::string content(void) const;

private:
        // invariants
        bool invariant(void) const;

        // implementation
        static const size_t MAX_LENGTH = 125;

        // representation
        char d_header;
        std::string d_content;
};
