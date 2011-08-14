#include <algorithm>    // std::for_each
#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Move.h"
#include "Parser.h"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string Move::HEADER = "M";

const bool Move::REGISTERED = Parser<protocol>::register_message(HEADER, create);

std::unique_ptr<MessageInterface> Move::create(const std::string& message)
{
        return std::unique_ptr<Move>(new Move(message));
}

Move::Move(const std::string& message)
:
        seconds_(boost::lexical_cast<int>(message.substr(0, 4).c_str())),
        from_sq_(boost::lexical_cast<int>(message.substr(4, 2).c_str())),
        dest_sq_(boost::lexical_cast<int>(message.substr(6, 2).c_str())),
        num_captured_(boost::lexical_cast<int>(message.substr(8, 2).c_str()))
{
        for (auto i = 0; i < num_captured(); ++i)
                captured_pieces_.push_back(boost::lexical_cast<int>(message.substr(10 + 2 * i, 2).c_str()));
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
        std::for_each(c.begin(), c.end(), [&sstr](int x) { 
                sstr << std::setw(2) << std::setfill('0') << x; 
        });
        return sstr.str();
}

}       // namespace dxp
}       // namespace dctl
