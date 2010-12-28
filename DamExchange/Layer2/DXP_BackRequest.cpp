#include "DXP_BackRequest.h"
#include "../../IO/Token.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

DXP_BackRequest::DXP_BackRequest(size_t m, bool c)
:
        d_move_number(m),
        d_side_to_move(c)
{
}

DXP_BackRequest::DXP_BackRequest(const std::string& s)
:
        d_move_number(atoi(s.substr(1, 3).c_str())),
        d_side_to_move(DXP_PositionToken::read_color( *(s.substr(4, 1)).begin() ))
{
        assert(invariant(*s.begin()));
}

char DXP_BackRequest::header(void) const
{
        return HEADER;
}

std::string DXP_BackRequest::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 3) << std::setfill('0') << move_number();
        sstr << std::setw( 1) << DXP_PositionToken::write_color(side_to_move());

        return sstr.str();
}

size_t DXP_BackRequest::move_number(void) const
{
        return d_move_number;
}

bool DXP_BackRequest::side_to_move(void) const
{
        return d_side_to_move;
}
