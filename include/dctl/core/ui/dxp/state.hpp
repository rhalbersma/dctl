#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/board.hpp>
#include <dctl/core/board/string.hpp>
#include <dctl/core/state/color_piece.hpp>
#include <dctl/core/state/basic_state.hpp>
#include <dctl/core/ui/basic_token_set.hpp>     // basic_token_set
#include <dctl/core/ui/color.hpp>               // read_color, write_color
#include <dctl/core/ui/position.hpp>            // content
#include <dctl/util/type_traits.hpp>            // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert
#include <cctype>                               // isupper, toupper
#include <sstream>                              // stringstream
#include <string>                               // string

namespace dctl::core {
namespace dxp {
inline namespace v1 {

using token_set = basic_token_set<'Z', 'W', 'e'>;

template<class Rules, class Board = board<Rules>, class Token = token_set>
struct read
{
        auto operator()(std::string const& s) const
                -> basic_state<Rules, Board>
        {
                using set_type = set_t<Board>;
                set_type by_color_piece[2][2]{};
                auto p_side = color::black;

                assert(by_color_piece[0][0].empty());
                assert(by_color_piece[0][1].empty());
                assert(by_color_piece[1][0].empty());
                assert(by_color_piece[1][1].empty());

                // do not attempt to parse empty strings
                if (s.empty()) {
                        return {{by_color_piece[0][0], by_color_piece[1][0], by_color_piece[0][1], by_color_piece[1][1]}, p_side};
                }

                std::stringstream sstr(s);
                char ch;
                sstr >> ch;
                p_side = read_color<Token>(ch);

                 for (auto sq = 0; sq < Board::size(); ++sq) {
                        auto b = Board::bit_from_square(sq);
                        sstr >> ch;
                        switch (std::toupper(ch)) {
                        case Token::black: by_color_piece[xstd::to_underlying_type(color::black)][std::isupper(ch)].insert(b); break;
                        case Token::white: by_color_piece[xstd::to_underlying_type(color::white)][std::isupper(ch)].insert(b); break;
                        case Token::empty: break;
                        default: assert(false);
                        }
                }
                return {{by_color_piece[0][0], by_color_piece[1][0], by_color_piece[0][1], by_color_piece[1][1]}, p_side};
        }
};

template<class Token = token_set>
struct write
{
        template<class State>
        auto operator()(State const& s) const
        {
                std::stringstream sstr;
                sstr << write_color<Token>(s.to_move());
                sstr << string<board_t<State>>{}([&](auto const n) { return content<Token>(s, n); });
                sstr << '\n';
                return sstr.str();
        }
};

}       // inline namespace v1
}       // namespace dxp
}       // namespace dctl::core
