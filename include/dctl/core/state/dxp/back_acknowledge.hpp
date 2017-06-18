#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <iomanip>              // setw
#include <sstream>              // stringstream
#include <string>               // stoi, string

namespace dctl::core {
namespace dxp {

/*

        The format and semantics of BACKACC are explained at:
        http://www.mesander.nl/damexchange/ebackacc.htm

*/

class back_acknowledge
{
public:
        enum class acceptance
        :
                int
        {
                accept          = 0,
                not_supported   = 1,
                decline         = 2
        };

private:
        inline static auto const s_header = "K";
        acceptance m_acceptance_code;

public:
        explicit back_acknowledge(std::string const& message)
        :
                m_acceptance_code{static_cast<acceptance>(std::stoi(message.substr(0, 1)))}
        {}

        static auto header() noexcept
        {
                return s_header;
        }

        auto acceptance_code() const noexcept
        {
                return m_acceptance_code;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(1) << xstd::to_underlying_type(acceptance_code());
                return sstr.str();
        }
};

}       // namespace dxp
}       // namespace dctl::core
