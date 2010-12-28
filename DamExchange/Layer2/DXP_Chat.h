#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_Chat: public DXP_AbstractMessage
{
public:
        // constructors
        DXP_Chat(const std::string&);

        // views
        char header(void) const;
        std::string message(void) const;

        // implementation
        static const char HEADER = 'C';

private:
        // representation
        std::string d_message;
};
