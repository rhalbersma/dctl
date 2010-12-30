#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_String.h"
#include <string>

class DXP_Chat: public DXP_AbstractMessage
{
public:
        // constructors
        static DXP_AbstractMessage* create(const DXP_String&);

        DXP_Chat(const std::string&);

        // views
        virtual DXP_String message(void) const;

private:
        // implementation
        static const char HEADER = 'C';
        static const bool REGISTERED;

        // representation
        std::string d_message;
};
