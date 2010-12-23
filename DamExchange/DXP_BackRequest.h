#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_BackRequest: public DXP_AbstractMessage
{
public:
        DXP_BackRequest(const std::string&);
        DXP_BackRequest(size_t, bool);

        std::string send_message(void) const;

        static const char HEADER = 'B';

private:    
        size_t move_number;
        bool side_to_move;
};
