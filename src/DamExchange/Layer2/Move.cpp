#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Move.h"
#include "Parser.h"

namespace damexchange {
namespace layer2 {

const std::string Move::HEADER = "M";

const bool Move::REGISTERED = Parser::insert(HEADER, create);

std::unique_ptr<MessageInterface> Move::create(const std::string& msg)
{
        return std::unique_ptr<Move>(new Move(msg));
}

Move::Move(const std::string& msg)
:
        seconds_(boost::lexical_cast<int>(msg.substr(0, 4).c_str())),
        from_sq_(boost::lexical_cast<int>(msg.substr(4, 2).c_str())),
        dest_sq_(boost::lexical_cast<int>(msg.substr(6, 2).c_str())),
        num_captured_(boost::lexical_cast<int>(msg.substr(8, 2).c_str()))
{
        for (auto i = 0; i < num_captured(); ++i)
                captured_pieces_.push_back(boost::lexical_cast<int>(msg.substr(10 + 2 * i, 2).c_str()));
}

int Move::seconds() const
{
        return seconds_;
}

int Move::from_sq() const
{
        return from_sq_;
}

int Move::dest_sq() const
{
        return dest_sq_;
}

int Move::num_captured() const
{
        return num_captured_;
}

const std::vector<int>& Move::captured_pieces() const
{
        return captured_pieces_;
}

std::string Move::str(int s, int f, int d, int n, const std::vector<int>& c)
{
        return HEADER + body(s, f, d, n, c);
}

std::string Move::header() const
{
        return HEADER;
}

std::string Move::body() const
{
        return body(seconds(), from_sq(), dest_sq(), num_captured(), captured_pieces());
}

std::string Move::body(int s, int f, int d, int n, const std::vector<int>& c)
{ 
        std::stringstream sstr;
        sstr << std::setw( 4) << std::setfill('0') << s;
        sstr << std::setw( 2) << std::setfill('0') << f;
        sstr << std::setw( 2) << std::setfill('0') << d;
        sstr << std::setw( 2) << std::setfill('0') << n;
        for (auto it = c.begin(); it != c.end(); ++it)
                sstr << std::setw(2) << std::setfill('0') << *it;
        return sstr.str();
}

}       // namespace layer2
}       // namespace damexchange
