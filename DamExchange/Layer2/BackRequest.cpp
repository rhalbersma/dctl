#include "BackRequest.h"
#include "Parser.h"
#include "Token.h"
#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace DXP = DamExchange;

const std::string DXP::Layer2::BackRequest::HEADER = "B";

const bool DXP::Layer2::BackRequest::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::Layer2::AbstractMessage> DXP::Layer2::BackRequest::create(const std::string& msg)
{
        return std::make_shared<BackRequest>(msg);
}

DXP::Layer2::BackRequest::BackRequest(const std::string& msg)
:
        move_number_(boost::lexical_cast<size_t>(msg.substr(0, 3).c_str())),
        side_to_move_(PositionToken<DXP_tag>::read_color( *(msg.substr(3, 1)).begin() ))
{
}

DXP::Layer2::BackRequest::BackRequest(size_t m, bool c)
:
        move_number_(m),
        side_to_move_(c)
{
}

size_t DXP::Layer2::BackRequest::move_number(void) const
{
        return move_number_;
}

bool DXP::Layer2::BackRequest::side_to_move(void) const
{
        return side_to_move_;
}

std::string DXP::Layer2::BackRequest::header(void) const
{
        return HEADER;
}

std::string DXP::Layer2::BackRequest::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 3) << std::setfill('0') << move_number();
        sstr << std::setw( 1) << PositionToken<DXP_tag>::write_color(side_to_move());
        return sstr.str();
}
