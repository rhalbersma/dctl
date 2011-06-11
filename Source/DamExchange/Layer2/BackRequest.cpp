#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "BackRequest.h"
#include "Parser.h"

namespace DXP = damexchange;

const std::string DXP::layer2::BackRequest::HEADER = "B";

const bool DXP::layer2::BackRequest::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::layer2::AbstractMessage> DXP::layer2::BackRequest::create(const std::string& msg)
{
        return std::make_shared<BackRequest>(msg);
}

DXP::layer2::BackRequest::BackRequest(const std::string& msg)
:
        move_number_(boost::lexical_cast<size_t>(msg.substr(0, 3).c_str())),
        side_to_move_(*(msg.substr(3, 1)).begin())
{
}

DXP::layer2::BackRequest::BackRequest(size_t m, char c)
:
        move_number_(m),
        side_to_move_(c)
{
}

size_t DXP::layer2::BackRequest::move_number(void) const
{
        return move_number_;
}

char DXP::layer2::BackRequest::side_to_move(void) const
{
        return side_to_move_;
}

std::string DXP::layer2::BackRequest::header(void) const
{
        return HEADER;
}

std::string DXP::layer2::BackRequest::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 3) << std::setfill('0') << move_number();
        sstr << std::setw( 1) << side_to_move();
        return sstr.str();
}
