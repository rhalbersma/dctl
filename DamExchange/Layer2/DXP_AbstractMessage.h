#pragma once
#include "DXP_StringMessage.h"

class DXP_AbstractMessage
{
public:
        virtual DXP_StringMessage message(void) const = 0;

        // virtual destructor
        virtual ~DXP_AbstractMessage(void) {};
};
