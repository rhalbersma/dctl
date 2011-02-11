#include "Move.h"
#include "Parser.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace DXP = DamExchangeProtocol;
namespace Layer2 = DXP::Message;

const std::string Layer2::Move::HEADER = "M";

const bool Layer2::Move::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<Layer2::AbstractMessage> Layer2::Move::create(const std::string& s)
{
        return std::make_shared<Move>(s);
}

Layer2::Move::Move(const std::string& s)
:
        seconds_(atoi(s.substr(0, 4).c_str())),
        from_sq_(atoi(s.substr(4, 2).c_str())),
        dest_sq_(atoi(s.substr(6, 2).c_str())),
        num_captured_(atoi(s.substr(8, 2).c_str()))
{
        for (size_t i = 0; i < num_captured(); ++i)
                captured_pieces_.push_back(atoi(s.substr(10 + 2 * i, 2).c_str()));
}

Layer2::Move::Move(size_t s, size_t f, size_t d, size_t n, const std::vector<size_t>& c)
:
        seconds_(s),
        from_sq_(f),
        dest_sq_(d),
        num_captured_(n),
        captured_pieces_(c)
{
}

size_t Layer2::Move::seconds(void) const
{
        return seconds_;
}

size_t Layer2::Move::from_sq(void) const
{
        return from_sq_;
}

size_t Layer2::Move::dest_sq(void) const
{
        return dest_sq_;
}

size_t Layer2::Move::num_captured(void) const
{
        return num_captured_;
}

const std::vector<size_t>& Layer2::Move::captured_pieces(void) const
{
        return captured_pieces_;
}

std::string Layer2::Move::header(void) const
{
        return HEADER;
}

std::string Layer2::Move::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 4) << std::setfill('0') << seconds();
        sstr << std::setw( 2) << std::setfill('0') << from_sq();
        sstr << std::setw( 2) << std::setfill('0') << dest_sq();
        sstr << std::setw( 2) << std::setfill('0') << num_captured();
        for (std::vector<size_t>::const_iterator it = captured_pieces().begin(); it != captured_pieces().end(); ++it)
                sstr << std::setw(2) << std::setfill('0') << *it;
        return sstr.str();
}
