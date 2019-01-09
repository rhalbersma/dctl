#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <chrono>       // minutes, seconds
#include <cstddef>      // size_t
#include <functional>   // function
#include <iomanip>      // setfill, setw
#include <map>          // map
#include <stdexcept>    // invalid_argument
#include <sstream>      // stringstream
#include <string>       // stoi, string
#include <variant>      // variant
#include <vector>       // vector

namespace dctl::core {
namespace dxp {
inline namespace v1 {

// The Layer 2 protocol description can be found at:
// http://www.mesander.nl/damexchange/edxplg2.htm

class gamereq
{
        // The format and semantics of the GAMEREQ message type can be found at:
        // http://www.mesander.nl/damexchange/egamereq.htm

        constexpr static auto s_header = "R";
        constexpr static auto s_version = 1;
        constexpr static auto s_color_to_move_first = 'W';
        constexpr static auto s_starting_position = "\
                  z   z   z   z   z \
                z   z   z   z   z   \
                  z   z   z   z   z \
                z   z   z   z   z   \
                  e   e   e   e   e \
                e   e   e   e   e   \
                  w   w   w   w   w \
                w   w   w   w   w   \
                  w   w   w   w   w \
                w   w   w   w   w   \
        ";
        int m_version;
        std::string m_initiator_name;
        char m_follower_color;
        std::chrono::minutes m_thinking_time;
        int m_number_of_moves;
        char m_starting_position;
        char m_color_to_move_first;
        std::string m_position;
public:
        explicit gamereq(std::string const& message)
        :
                m_version{std::stoi(message.substr(0, 2))},
                m_initiator_name{message.substr(2, 32)},
                m_follower_color{message[34]},
                m_thinking_time{std::stoi(message.substr(35, 3))},
                m_number_of_moves{std::stoi(message.substr(38, 3))},
                m_starting_position{message[41]}
        {
                if (m_starting_position == 'A') {
                        m_color_to_move_first = s_color_to_move_first;
                        m_position = s_starting_position;
                } else if (m_starting_position == 'B') {
                        m_color_to_move_first = message[42];
                        m_position = message.substr(43, 50);
                }
        }

        gamereq(std::string const& in, char const fc, std::chrono::minutes const tt, int const nofm)
        :
                m_version{s_version},
                m_initiator_name{in},
                m_follower_color{fc},
                m_thinking_time{tt},
                m_number_of_moves{nofm},
                m_starting_position{'A'},
                m_color_to_move_first{s_color_to_move_first},
                m_position{s_starting_position}
        {}

        gamereq(std::string const& in, char const fc, std::chrono::minutes const tt, int const nofm, char const ctmf, std::string const& p)
        :
                m_version{s_version},
                m_initiator_name{in},
                m_follower_color{fc},
                m_thinking_time{tt},
                m_number_of_moves{nofm},
                m_starting_position{'B'},
                m_color_to_move_first{ctmf},
                m_position{p}
        {}

        constexpr static auto header() noexcept
        {
                return s_header;
        }

        auto version() const
        {
                return m_version;
        }

        auto initiator_name() const
        {
                return m_initiator_name;
        }

        auto follower_color() const
        {
                return m_follower_color;
        }

        auto thinking_time() const
        {
                return m_thinking_time;
        }

        auto number_of_moves() const
        {
                return m_number_of_moves;
        }

        auto starting_position() const
        {
                return m_starting_position;
        }

        auto color_to_move_first() const
        {
                return m_color_to_move_first;
        }

        auto position() const
        {
                return m_position;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(2) << std::setfill('0') << version();
                sstr << initiator_name();
                sstr << follower_color();
                sstr << std::setw(3) << thinking_time().count();
                sstr << std::setw(3) << number_of_moves();
                sstr << starting_position();
                if (starting_position() == 'B') {
                        sstr << color_to_move_first();
                        sstr << position();
                }
                return sstr.str();
        }
};

class gameacc
{
        // The format and semantics of the GAMEACC message type can be found at:
        // http://www.mesander.nl/damexchange/egameacc.htm

        constexpr static auto s_header = "A";
        std::string m_follower_name;
        int m_acceptance_code;
public:
        explicit gameacc(std::string const& message)
        :
                m_follower_name{message.substr(0, 32)},
                m_acceptance_code{std::stoi(message.substr(32, 1))}
        {}

        gameacc(std::string const& fn, int const ac)
        :
                m_follower_name{fn},
                m_acceptance_code{ac}
        {}

        constexpr static auto header() noexcept
        {
                return s_header;
        }

        auto follower_name() const
        {
                return m_follower_name;
        }

        auto acceptance_code() const noexcept
        {
                return m_acceptance_code;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << follower_name();
                sstr << std::setw(1) << acceptance_code();
                return sstr.str();
        }
};

class move
{
        // The format and semantics of the MOVE message type can be found at:
        // http://www.mesander.nl/damexchange/emove.htm

        constexpr static auto s_header = "M";
        std::chrono::seconds m_time;
        int m_from_field;
        int m_to_field;
        std::vector<int> m_captured_pieces;
public:
        explicit move(std::string const& message)
        :
                m_time{std::stoi(message.substr(0, 4))},
                m_from_field{std::stoi(message.substr(4, 2))},
                m_to_field{std::stoi(message.substr(6, 2))}
        {
                for (auto i = 0, n = std::stoi(message.substr(8, 2)); i < n; ++i) {
                        auto const index = static_cast<std::size_t>(10 + 2 * i);
                        m_captured_pieces.push_back(std::stoi(message.substr(index, 2)));
                }
        }

        move(std::chrono::seconds const t, int const ff, int const tf, std::vector<int> const& cp)
        :
                m_time{t},
                m_from_field{ff},
                m_to_field{tf},
                m_captured_pieces{cp}
        {}

        constexpr static auto header() noexcept
        {
                return s_header;
        }

        auto time() const noexcept
        {
                return m_time;
        }

        auto from_field() const noexcept
        {
                return m_from_field;
        }

        auto to_field() const noexcept
        {
                return m_to_field;
        }

        auto number_captured() const noexcept
        {
                return static_cast<int>(m_captured_pieces.size());
        }

        auto captured_pieces() const
        {
                return m_captured_pieces;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(4) << std::setfill('0') << time().count();
                sstr << std::setw(2) << std::setfill('0') << from_field();
                sstr << std::setw(2) << std::setfill('0') << to_field();
                sstr << std::setw(2) << std::setfill('0') << number_captured();
                for (auto const c : captured_pieces()) {
                        sstr << std::setw(2) << std::setfill('0') << c;
                }
                return sstr.str();
        }
};

class gameend
{
        // The format and semantics of the GAMEEND message type can be found at:
        // http://www.mesander.nl/damexchange/egameend.htm

        constexpr static auto s_header = "E";
        int m_reason;
        int m_stop_code;
public:
        explicit gameend(std::string const& message)
        :
                m_reason{std::stoi(message.substr(0, 1))},
                m_stop_code{std::stoi(message.substr(1, 1))}
        {}

        gameend(int const r, int const sc) noexcept
        :
                m_reason{r},
                m_stop_code{sc}
        {}

        constexpr static auto header() noexcept
        {
                return s_header;
        }

        auto reason() const noexcept
        {
                return m_reason;
        }

        auto stop_code() const noexcept
        {
                return m_stop_code;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(1) << reason();
                sstr << std::setw(1) << stop_code();
                return sstr.str();
        }
};

class chat
{
        // The format and semantics of the CHAT message type can be found at:
        // http://www.mesander.nl/damexchange/echat.htm

        constexpr static auto s_header = "C";
        std::string m_text;
public:
        explicit chat(std::string const& message)
        :
                m_text{message}
        {}

        constexpr static auto header() noexcept
        {
                return s_header;
        }

        auto text() const
        {
                return m_text;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << text();
                return sstr.str();
        }
};

class backreq
{
        // The format and semantics of the BACKREQ message type can be found at:
        // http://www.mesander.nl/damexchange/ebackreq.htm

        constexpr static auto s_header = "B";
        int m_move_number;
        char m_color_to_move;
public:
        explicit backreq(std::string const& message)
        :
                m_move_number{std::stoi(message.substr(0, 3))},
                m_color_to_move{message[3]}
        {}

        backreq(int const mn, char const ctm) noexcept
        :
                m_move_number{mn},
                m_color_to_move{ctm}
        {}

        constexpr static auto header() noexcept
        {
                return s_header;
        }

        auto move_number() const noexcept
        {
                return m_move_number;
        }

        auto color_to_move() const noexcept
        {
                return m_color_to_move;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(3) << std::setfill('0') << move_number();
                sstr << color_to_move();
                return sstr.str();
        }
};

class backacc
{
        // The format and semantics of the BACKACC message type can be found at:
        // http://www.mesander.nl/damexchange/ebackacc.htm

        constexpr static auto s_header = "K";
        int m_acceptance_code;
public:
        explicit backacc(std::string const& message)
        :
                m_acceptance_code{std::stoi(message.substr(0, 1))}
        {}

        explicit backacc(int const ac) noexcept
        :
                m_acceptance_code{ac}
        {}

        constexpr static auto header() noexcept
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
                sstr << std::setw(1) << acceptance_code();
                return sstr.str();
        }
};

template<class... Ts>
class basic_factory
{
        std::map<std::string, std::function<std::variant<Ts...>(std::string)>> m_registry;
public:
        basic_factory()
        :
                m_registry{{Ts::header(), [](auto const& arg){ return Ts(arg); }}...}
        {}

        auto operator()(std::string const& arg) const
        {
                if (auto const it = m_registry.find(arg.substr(0, 1)); it != m_registry.end()) {
                        return (it->second)(arg.substr(1));
                } else {
                        throw std::invalid_argument(arg);
                }
        }
};

using factory = basic_factory<gamereq, gameacc, move, gameend, chat, backreq, backacc>;

}       // inline namespace v1
}       // namespace dxp
}       // namespace dctl::core
