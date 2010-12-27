#pragma once
#include <string>

class DXP_AbstractMessage
{
public:
        virtual std::string message(void) const = 0;
};
