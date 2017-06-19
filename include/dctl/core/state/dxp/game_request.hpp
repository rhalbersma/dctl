#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <iomanip>              // setfill, setw
#include <sstream>              // stringstream
#include <string>               // stoi, string

namespace dctl::core {
namespace dxp {

/*

        The format and semantics of GAMEREQ are defined at:
        http://www.mesander.nl/damexchange/egamereq.htm

*/

class game_request
{
public:
        enum class setup
        :
                char
        {
                initial = 'A',
                special = 'B'
        };

private:
        inline static auto const s_header = "R";
        constexpr static auto s_protocol_version = 1;
        std::string m_name_initiator;
        char m_color_follower;
        int m_minutes;
        int m_moves;
        setup m_setup_code;
        std::string m_position = "Wzzzzzzzzzzzzzzzzzzzzeeeeeeeeeewwwwwwwwwwwwwwwwwwww";

public:
        explicit game_request(std::string const& message)
        :
                m_name_initiator{message.substr(2, 32)},
                m_color_follower{*(std::begin(message.substr(34, 1)))},
                m_minutes{std::stoi(message.substr(35, 3))},
                m_moves{std::stoi(message.substr(38, 3))},
                m_setup_code{static_cast<setup>(*(std::begin(message.substr(41, 1))))}
        {
                if (m_setup_code == setup::special) {
                        m_position = message.substr(42, 51);
                }
        }

        static auto header() noexcept
        {
                return s_header;
        }

        constexpr static auto protocol_version() noexcept
        {
                return s_protocol_version;
        }

        auto name_initiator() const
        {
                return m_name_initiator;
        }

        auto color_follower() const noexcept
        {
                return m_color_follower;
        }

        auto minutes() const noexcept
        {
                return m_minutes;
        }

        auto moves() const noexcept
        {
                return m_moves;
        }

        auto setup_code() const noexcept
        {
                return m_setup_code;
        }

        auto position() const
        {
                return m_position;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw( 2) << std::setfill('0') << protocol_version();
                sstr << std::setw(32) << name_initiator() << std::setfill(' ');
                sstr << std::setw( 1) << color_follower();
                sstr << std::setw( 3) << std::setfill('0') << minutes();
                sstr << std::setw( 3) << std::setfill('0') << moves();
                sstr << std::setw( 1) << xstd::to_underlying_type(setup_code());
                if (setup_code() == setup::special) {
                        sstr << std::setw(51) << position();
                }
                return sstr.str();
        }
};

}       // namespace dxp
}       // namespace dctl::core
