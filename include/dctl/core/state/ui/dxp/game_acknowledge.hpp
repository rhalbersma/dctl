#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <iomanip>              // setfill, setw
#include <sstream>              // stringstream
#include <string>               // stoi, string

namespace dctl::core {
namespace dxp {

/*

        The format and semantics of GAMEACC are defined at:
        http://www.mesander.nl/damexchange/egameacc.htm

*/

class game_acknowledge
{
public:
        enum class acceptance
        :
                int
        {
                accept          = 0,
                decline_version = 1,
                decline_game    = 2,
                decline_always  = 3
        };

private:
        inline static auto const s_header = "A";
        std::string m_name_follower;
        acceptance m_acceptance_code;

public:
        explicit game_acknowledge(std::string const& message)
        :
                m_name_follower{message.substr(0, 32)},
                m_acceptance_code{static_cast<acceptance>(std::stoi(message.substr(32, 1).c_str()))}
        {}

        static auto header() noexcept
        {
                return s_header;
        }

        auto name_follower() const
        {
                return m_name_follower;
        }

        auto acceptance_code() const noexcept
        {
                return m_acceptance_code;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(32) << name_follower() << std::setfill(' ');
                sstr << std::setw( 1) << xstd::to_underlying_type(acceptance_code());
                return sstr.str();
        }
};

}       // namespace dxp
}       // namespace dctl::core
