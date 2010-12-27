#include "DXP_Move.h"
#include <cstdlib>
#include <iomanip>
#include <sstream>

DXP_Move::DXP_Move(size_t s, size_t f, size_t d, size_t n, const std::vector<size_t>& c)
:
        d_seconds(s),
        d_from_sq(f),
        d_dest_sq(d),
        d_num_captured(n),
        d_captured_pieces(c)
{
}

DXP_Move::DXP_Move(const std::string& s)
:
        d_seconds(atoi(s.substr(1, 4).c_str())),
        d_from_sq(atoi(s.substr(5, 2).c_str())),
        d_dest_sq(atoi(s.substr(7, 2).c_str())),
        d_num_captured(atoi(s.substr(9, 2).c_str()))

{
        for(size_t i = 0; i < num_captured(); ++i)
                d_captured_pieces.push_back(atoi(s.substr(11 + 2 * i, 2).c_str()));
}

std::string DXP_Move::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 4) << std::setfill('0') << seconds();
        sstr << std::setw( 2) << std::setfill('0') << from_sq();
        sstr << std::setw( 2) << std::setfill('0') << dest_sq();
        sstr << std::setw( 2) << std::setfill('0') << num_captured();
        for(std::vector<size_t>::const_iterator it = captured_pieces().begin(); it != captured_pieces().end(); ++it)
                sstr << std::setw(2) << std::setfill('0') << *it;

        return sstr.str();
}

size_t DXP_Move::seconds(void) const
{
        return d_seconds;
}

size_t DXP_Move::from_sq(void) const
{
        return d_from_sq;
}

size_t DXP_Move::dest_sq(void) const
{
        return d_dest_sq;
}

size_t DXP_Move::num_captured(void) const
{
        return d_num_captured;
}

const std::vector<size_t>& DXP_Move::captured_pieces(void) const
{
        return d_captured_pieces;
}
