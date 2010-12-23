#pragma once
#include <string>

class DXP_AbstractMessage
{
public:
        virtual std::string send_message(void) const = 0;
};
