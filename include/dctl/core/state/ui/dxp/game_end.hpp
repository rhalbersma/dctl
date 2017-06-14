#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <iomanip>              // setw
#include <sstream>              // stringstream
#include <string>               // stoi, string

namespace dctl::core {
namespace dxp {

/*

        The format and semantics of GAMEEND are defined at:
        http://www.mesander.nl/damexchange/egameend.htm

*/

class game_end
{
public:
        enum class reason
        :
                int
        {
                forfeit         = 0,
                resign          = 1,
                claim_draw      = 2,
                claim_win       = 3
        };

        enum class stop
        :
                int
        {
                game    = 0,
                always  = 1
        };

private:
        inline static auto const s_header = "E";
        reason m_reason_code;
        stop m_stop_code;

public:
        explicit game_end(std::string const& message)
        :
                m_reason_code{static_cast<reason>(std::stoi(message.substr(0, 1).c_str()))},
                m_stop_code{static_cast<stop>(std::stoi(message.substr(1, 1).c_str()))}
        {}

        static auto header() noexcept
        {
                return s_header;
        }

        auto reason_code() const noexcept
        {
                return m_reason_code;
        }

        auto stop_code() const noexcept
        {
                return m_stop_code;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(1) << xstd::to_underlying_type(reason_code());
                sstr << std::setw(1) << xstd::to_underlying_type(stop_code());
                return sstr.str();
        }
};

}       // namespace dxp
}       // namespace dctl::core
