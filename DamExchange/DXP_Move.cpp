#include "DXP_Move.h"
#include <iomanip>
#include <sstream>

DXP_Move::DXP_Move(const std::string&)
{
}

std::string DXP_Move::send_message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 4) << std::setfill('0') << seconds_for_move;
        sstr << std::setw( 2) << std::setfill('0') << from_sq;
        sstr << std::setw( 2) << std::setfill('0') << dest_sq;
        sstr << std::setw( 2) << std::setfill('0') << num_captured;
        sstr << captured_pieces;

        return sstr.str();
}
