#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>      // assert
#include <sstream>      // stringstream
#include <string>       // string

namespace dctl::core {
namespace dxp {

/*

        The format and semantics of CHAT are defined at:
        http://www.mesander.nl/damexchange/echat.htm

*/

class chat
{
        inline static auto const s_header = "C";
        std::string m_text;

        auto assert_invariants() const noexcept
        {
                assert(m_text.size() <= 126);
        }
public:
        explicit chat(std::string const& message)
        :
                m_text{message}
        {
                assert_invariants();
        }

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

}       // namespace dxp
}       // namespace dctl::core
