#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iomanip>      // setfill, setw
#include <sstream>      // stringstream
#include <string>       // stoi, string

namespace dctl::core {
namespace dxp {

/*

        The format and semantics of BACKREQ are explained at:
        http://www.mesander.nl/damexchange/ebackreq.htm

*/

class back_request
{
        inline static auto const s_header = "B";
        int m_move_number;
        char m_side_to_move;
public:
        explicit back_request(std::string const& message)
        :
                m_move_number{std::stoi(message.substr(0, 3))},
                m_side_to_move{*(std::begin(message.substr(3, 1)))}
        {}

        static auto header() noexcept
        {
                return s_header;
        }

        auto move_number() const noexcept
        {
                return m_move_number;
        }

        auto side_to_move() const noexcept
        {
                return m_side_to_move;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(3) << std::setfill('0') << move_number();
                sstr << std::setw(1) << side_to_move();
                return sstr.str();
        }
};

}       // namespace dxp
}       // namespace dctl::core
