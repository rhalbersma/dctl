#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_Chat: public DXP_AbstractMessage
{
public:
        DXP_Chat(const std::string&);

        std::string send_message(void) const;

        static const char HEADER = 'C';

private:    
        std::string message;
};
