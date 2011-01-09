#include "DXP_BackRequest.h"
#include "DXP_MessageFactory.h"
#include "../IO/Token.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const std::string DXP_BackRequest::HEADER = "B";

const bool DXP_BackRequest::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_BackRequest::create(const DXP_StringMessage& s)
{
        assert(s.header() == HEADER);
        return new DXP_BackRequest(s.body());
}

DXP_BackRequest::DXP_BackRequest(const std::string& s)
:
        move_number_(atoi(s.substr(0, 3).c_str())),
        side_to_move_(DXP_PositionToken::read_color( *(s.substr(3, 1)).begin() ))
{
}

DXP_BackRequest::DXP_BackRequest(size_t m, bool c)
:
        move_number_(m),
        side_to_move_(c)
{
}

DXP_StringMessage DXP_BackRequest::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 3) << std::setfill('0') << move_number();
        sstr << std::setw( 1) << DXP_PositionToken::write_color(side_to_move());

        return DXP_StringMessage(HEADER, sstr.str());
}

size_t DXP_BackRequest::move_number(void) const
{
        return move_number_;
}

bool DXP_BackRequest::side_to_move(void) const
{
        return side_to_move_;
}
