#include "DXP_String.h"
#include <cassert>

DXP_String::DXP_String(const std::string& s)
:
        d_header(*s.begin()),
        d_content(s.substr(1))
{
        assert(invariant());
}

DXP_String::DXP_String(char h, const std::string& c)
:
        d_header(h),
        d_content(c)
{
        assert(invariant());
}

std::string DXP_String::str(void) const
{
        return std::string(1, d_header).append(d_content);
}

char DXP_String::header(void) const
{
        return d_header;
}

std::string DXP_String::content(void) const
{
        return d_content;
}

bool DXP_String::invariant(void) const
{
        return d_content.length() <= MAX_LENGTH;
}
