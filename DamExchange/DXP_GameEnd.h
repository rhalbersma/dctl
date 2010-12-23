#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_GameEnd: public DXP_AbstractMessage
{
public:
        enum StopCode { STOP_THIS = 0, STOP_ALL = 1 };

        DXP_GameEnd(const std::string&);
        DXP_GameEnd(size_t, StopCode);

        std::string send_message(void) const;

        static const char HEADER = 'E';

private:    
        size_t reason;
        StopCode stop_code;
};
