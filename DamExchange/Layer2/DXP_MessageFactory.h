#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_MessageFactory
{
public:
        static const DXP_AbstractMessage* create(const std::string&);
};
