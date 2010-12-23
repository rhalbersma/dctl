#include "DXP_GameRequest.h"
#include <iomanip>
#include <sstream>

DXP_GameRequest::DXP_GameRequest(const std::string&)
{
}

std::string DXP_GameRequest::send_message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 2) << std::setfill('0') << PROTOCOL_VERSION;
        sstr << std::setw(32) << name_initiator << std::setfill(' ');
        sstr << std::setw( 1) << color_follower;
        sstr << std::setw( 3) << std::setfill('0') << minutes_for_game;
        sstr << std::setw( 3) << std::setfill('0') << moves_for_game;
        sstr << std::setw( 1) << initial_position;
        sstr << std::setw( 1) << side_to_move;
        sstr << std::setw(50) << setup_position;

        return sstr.str();
}
