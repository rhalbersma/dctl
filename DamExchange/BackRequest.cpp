#include "BackRequest.h"
#include "MessageFactory.h"
#include "Token.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace DXP = DamExchangeProtocol;

const std::string DXP::BackRequest::HEADER = "B";

const bool DXP::BackRequest::REGISTERED = MessageFactory::register_creator(HEADER, create);

std::shared_ptr<DXP::AbstractMessage> DXP::BackRequest::create(const StringMessage& s)
{
        assert(REGISTERED);
        assert(s.header() == HEADER);
        return std::make_shared<BackRequest>(s.body());
}

DXP::BackRequest::BackRequest(const std::string& s)
:
        move_number_(atoi(s.substr(0, 3).c_str())),
        side_to_move_(PositionToken<DXP_tag>::read_color( *(s.substr(3, 1)).begin() ))
{
}

DXP::BackRequest::BackRequest(size_t m, bool c)
:
        move_number_(m),
        side_to_move_(c)
{
}

size_t DXP::BackRequest::move_number(void) const
{
        return move_number_;
}

bool DXP::BackRequest::side_to_move(void) const
{
        return side_to_move_;
}

std::string DXP::BackRequest::header(void) const
{
        return HEADER;
}

std::string DXP::BackRequest::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 3) << std::setfill('0') << move_number();
        sstr << std::setw( 1) << PositionToken<DXP_tag>::write_color(side_to_move());
        return sstr.str();
}
