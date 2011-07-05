#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "BackAcknowledge.h"
#include "Parser.h"

namespace damexchange {
namespace layer2 {

const std::string BackAcknowledge::HEADER = "K";

const bool BackAcknowledge::REGISTERED = Parser::insert(HEADER, create);

std::unique_ptr<MessageInterface> BackAcknowledge::create(const std::string& msg)
{
        return std::unique_ptr<BackAcknowledge>(new BackAcknowledge(msg));
}

BackAcknowledge::BackAcknowledge(const std::string& msg)
:
        acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<size_t>(msg.substr(0, 1).c_str())))
{
}

BackAcknowledge::AcceptanceCode BackAcknowledge::acceptance_code() const
{
        return acceptance_code_;
}

std::string BackAcknowledge::str(AcceptanceCode a)
{
        return HEADER + body(a);
}

std::string BackAcknowledge::header() const
{
        return HEADER;
}

std::string BackAcknowledge::body() const
{
        return body(acceptance_code());
}

std::string BackAcknowledge::body(AcceptanceCode a)
{
        std::stringstream sstr;
        sstr << std::setw( 1) << a;
        return sstr.str();
}

}       // namespace layer2
}       // namespace damexchange
