#include "GameAcknowledge.h"
#include "Parser.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace DXP = DamExchangeProtocol;
namespace Layer2 = DXP::Message;

const std::string Layer2::GameAcknowledge::HEADER = "A";

const bool Layer2::GameAcknowledge::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<Layer2::AbstractMessage> Layer2::GameAcknowledge::create(const std::string& s)
{
        return std::make_shared<GameAcknowledge>(s);
}

Layer2::GameAcknowledge::GameAcknowledge(const std::string& s)
:
        name_follower_(s.substr(0, 32)),
        acceptance_code_(static_cast<AcceptanceCode>(atoi(s.substr(32, 1).c_str())))
{
}

Layer2::GameAcknowledge::GameAcknowledge(const std::string& n, AcceptanceCode a)
:
        name_follower_(n),
        acceptance_code_(a)
{
}

const std::string& Layer2::GameAcknowledge::name_follower(void) const
{
        return name_follower_;
}

Layer2::GameAcknowledge::AcceptanceCode Layer2::GameAcknowledge::acceptance_code(void) const
{
        return acceptance_code_;
}

std::string Layer2::GameAcknowledge::header(void) const
{
        return HEADER;
}

std::string Layer2::GameAcknowledge::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw(32) << name_follower() << std::setfill(' ');
        sstr << std::setw( 1) << acceptance_code();
        return sstr.str();
}
