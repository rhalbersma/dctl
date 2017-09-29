#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <chrono>       // minutes, seconds
#include <iomanip>      // setfill, setw
#include <sstream>      // stringstream
#include <string>       // stoi, string
#include <vector>       // vector

namespace dctl::core {
namespace dxp {

// The format and semantics of GAMEREQ are defined at:
// http://www.mesander.nl/damexchange/egamereq.htm

class game_request
{
        inline static auto const s_header = "R";
        int m_version;
        std::string m_initiator_name;
        std::string m_follower_color;
        std::chrono::minutes m_thinking_time;
        int m_number_of_moves;
        std::string m_starting_position;
        std::string m_color_to_move_first;
        std::string m_position;
public:
        explicit game_request(std::string const& message)
        :
                m_version{std::stoi(message.substr(0, 2))},
                m_initiator_name{message.substr(2, 32)},
                m_follower_color{message.substr(34, 1)},
                m_thinking_time{std::stoi(message.substr(35, 3))},
                m_number_of_moves{std::stoi(message.substr(38, 3))},
                m_starting_position{message.substr(41, 1)}
        {
                if (m_starting_position == "A") {
                        m_color_to_move_first = "W";
                        m_position = std::string(20, 'z') + std::string(10, 'e') + std::string(20, 'w');
                } else if (m_starting_position == "B") {
                        m_color_to_move_first = message.substr(42, 1);
                        m_position = message.substr(43, 50);
                }
        }

        static auto header() noexcept
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
                if (starting_position() == "B") {
                        sstr << color_to_move_first();
                        sstr << position();
                }
                return sstr.str();
        }
};

// The format and semantics of GAMEACC are defined at:
// http://www.mesander.nl/damexchange/egameacc.htm

class game_acknowledge
{
        inline static auto const s_header = "A";
        std::string m_follower_name;
        int m_acceptance_code;
public:
        explicit game_acknowledge(std::string const& message)
        :
                m_follower_name{message.substr(0, 32)},
                m_acceptance_code{std::stoi(message.substr(32, 1))}
        {}

        static auto header() noexcept
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

// The format and semantics of MOVE are defined at:
// http://www.mesander.nl/damexchange/emove.htm

class move
{
        inline static auto const s_header = "M";
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

        static auto header() noexcept
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

// The format and semantics of GAMEEND are defined at:
// http://www.mesander.nl/damexchange/egameend.htm

class game_end
{
        inline static auto const s_header = "E";
        int m_reason;
        int m_stop_code;
public:
        explicit game_end(std::string const& message)
        :
                m_reason{std::stoi(message.substr(0, 1))},
                m_stop_code{std::stoi(message.substr(1, 1))}
        {}

        static auto header() noexcept
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

// The format and semantics of CHAT are defined at:
// http://www.mesander.nl/damexchange/echat.htm

class chat
{
        inline static auto const s_header = "C";
        std::string m_text;
public:
        explicit chat(std::string const& message)
        :
                m_text{message}
        {}

        static auto header() noexcept
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

// The format and semantics of BACKREQ are explained at:
// http://www.mesander.nl/damexchange/ebackreq.htm

class back_request
{
        inline static auto const s_header = "B";
        int m_move_number;
        std::string m_color_to_move;
public:
        explicit back_request(std::string const& message)
        :
                m_move_number{std::stoi(message.substr(0, 3))},
                m_color_to_move{message.substr(3, 1)}
        {}

        static auto header() noexcept
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

// The format and semantics of BACKACC are explained at:
// http://www.mesander.nl/damexchange/ebackacc.htm

class back_acknowledge
{
        inline static auto const s_header = "K";
        int m_acceptance_code;
public:
        explicit back_acknowledge(std::string const& message)
        :
                m_acceptance_code{std::stoi(message.substr(0, 1))}
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
                sstr << std::setw(1) << acceptance_code();
                return sstr.str();
        }
};

}       // namespace dxp
}       // namespace dctl::core
