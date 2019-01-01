#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//#include <dctl/core/board.hpp>          // board
//#include <dctl/core/rules.hpp>          // international
#include <dctl/core/ui/dxp/message.hpp> // factory
//#include <dctl/core/ui/dxp/state.hpp>   // basic_state, read
//#include <stdexcept>                    // invalid_argument
#include <string>                       // string

struct Fixture
{
        dctl::core::dxp::factory parse;
/*
        struct
        {
                auto operator()(dctl::core::dxp::gamereq const& gr) const
                {
                        auto const state_str = std::string(1, gr.color_to_move_first()) + gr.position();
                        return dctl::core::dxp::read<dctl::core::international>{}(state_str);
                }

                template<class T>
                [[noreturn]] auto operator()(T const&) const
                        -> dctl::core::basic_state<dctl::core::international, dctl::core::basic_board<dctl::core::international>>
                {
                        throw std::invalid_argument("Did not match a DXP gamereq type.");
                }
        } to_state;
*/
};
