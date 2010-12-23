#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_Move: public DXP_AbstractMessage
{
public:
        DXP_Move(const std::string&);
        DXP_Move(size_t, size_t, size_t, size_t, const std::string&);

        std::string send_message(void) const;

        static const char HEADER = 'M';

private:    
        size_t seconds_for_move;
        size_t from_sq;
        size_t dest_sq;
        size_t num_captured;
        std::string captured_pieces;
};
