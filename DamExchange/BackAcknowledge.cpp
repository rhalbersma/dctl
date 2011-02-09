#include "BackAcknowledge.h"
#include "Factory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace DXP = DamExchangeProtocol;

const std::string DXP::BackAcknowledge::HEADER = "K";

const bool DXP::BackAcknowledge::REGISTERED = Factory::register_creator(HEADER, create);

std::shared_ptr<DXP::AbstractMessage> DXP::BackAcknowledge::create(const std::string& s)
{
        return std::make_shared<BackAcknowledge>(s);
}

DXP::BackAcknowledge::BackAcknowledge(const std::string& s)
:
        acceptance_code_(static_cast<AcceptanceCode>(atoi(s.substr(0, 1).c_str())))
{
}

DXP::BackAcknowledge::BackAcknowledge(AcceptanceCode a)
:
        acceptance_code_(a)
{
}

DXP::BackAcknowledge::AcceptanceCode DXP::BackAcknowledge::acceptance_code(void) const
{
        return acceptance_code_;
}

std::string DXP::BackAcknowledge::header(void) const
{
        return HEADER;
}

std::string DXP::BackAcknowledge::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 1) << acceptance_code();
        return sstr.str();
}
