#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_StringMessage.h"
#include <string>

class DXP_Chat: public DXP_AbstractMessage
{
public:
        // constructors
        static DXP_AbstractMessage* create(const DXP_StringMessage&);

        explicit DXP_Chat(const std::string&);

        // views
        virtual DXP_StringMessage message(void) const;

private:
        // implementation
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string message_;
};
