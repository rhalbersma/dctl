#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "BackAcknowledge.h"
#include "Parser.h"
#include "Version.h"

namespace dctl {
namespace protocol {
namespace damexchange {

const std::string BackAcknowledge::HEADER = "K";

const bool BackAcknowledge::REGISTERED = Parser<version>::insert(HEADER, create);

std::unique_ptr<MessageInterface> BackAcknowledge::create(const std::string& message)
{
        return std::unique_ptr<BackAcknowledge>(new BackAcknowledge(message));
}

BackAcknowledge::BackAcknowledge(const std::string& message)
:
        acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<size_t>(message.substr(0, 1).c_str())))
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
        sstr << std::setw(1) << a;
        return sstr.str();
}

}       // namespace damexchange
}       // namespace protocol
}       // namespace dctl
