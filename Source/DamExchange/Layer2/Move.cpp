#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Move.h"
#include "Parser.h"

namespace DXP = damexchange;

const std::string DXP::layer2::Move::HEADER = "M";

const bool DXP::layer2::Move::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::layer2::MessageInterface> DXP::layer2::Move::create(const std::string& msg)
{
        return std::make_shared<Move>(msg);
}

DXP::layer2::Move::Move(const std::string& msg)
:
        seconds_(boost::lexical_cast<int>(msg.substr(0, 4).c_str())),
        from_sq_(boost::lexical_cast<int>(msg.substr(4, 2).c_str())),
        dest_sq_(boost::lexical_cast<int>(msg.substr(6, 2).c_str())),
        num_captured_(boost::lexical_cast<int>(msg.substr(8, 2).c_str()))
{
        for (int i = 0; i < num_captured(); ++i)
                captured_pieces_.push_back(boost::lexical_cast<int>(msg.substr(10 + 2 * i, 2).c_str()));
}

DXP::layer2::Move::Move(int s, int f, int d, int n, const std::vector<int>& c)
:
        seconds_(s),
        from_sq_(f),
        dest_sq_(d),
        num_captured_(n),
        captured_pieces_(c)
{
}

int DXP::layer2::Move::seconds(void) const
{
        return seconds_;
}

int DXP::layer2::Move::from_sq(void) const
{
        return from_sq_;
}

int DXP::layer2::Move::dest_sq(void) const
{
        return dest_sq_;
}

int DXP::layer2::Move::num_captured(void) const
{
        return num_captured_;
}

const std::vector<int>& DXP::layer2::Move::captured_pieces(void) const
{
        return captured_pieces_;
}

std::string DXP::layer2::Move::header(void) const
{
        return HEADER;
}

std::string DXP::layer2::Move::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 4) << std::setfill('0') << seconds();
        sstr << std::setw( 2) << std::setfill('0') << from_sq();
        sstr << std::setw( 2) << std::setfill('0') << dest_sq();
        sstr << std::setw( 2) << std::setfill('0') << num_captured();
        for (std::vector<int>::const_iterator it = captured_pieces().begin(); it != captured_pieces().end(); ++it)
                sstr << std::setw(2) << std::setfill('0') << *it;
        return sstr.str();
}
