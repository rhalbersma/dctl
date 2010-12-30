#pragma once
#include "DXP_String.h"

class DXP_AbstractMessage
{
public:
        virtual DXP_String message(void) const = 0;

        // virtual destructor
        virtual ~DXP_AbstractMessage(void) {};
};
