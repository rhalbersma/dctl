#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/basic_board.hpp>
#include <dctl/core/board/string.hpp>
#include <dctl/core/state/basic_state.hpp>
#include <dctl/core/state/color.hpp>
#include <dctl/core/state/piece.hpp>
#include <dctl/core/ui/basic_token_set.hpp>     // basic_token_set
#include <dctl/core/ui/color.hpp>               // read_color, write_color
#include <dctl/core/ui/content.hpp>             // content
#include <dctl/util/type_traits.hpp>            // set_t
#include <cassert>                              // assert
#include <cctype>                               // islower, isupper, tolower, toupper
#include <sstream>                              // stringstream
#include <string>                               // string

namespace dctl::core {
namespace dxp {
inline namespace v1 {

using token_set = basic_token_set<'Z', 'W', 'e'>;

template<class Rules, class Board = basic_board<Rules>, class Token = token_set>
struct read
{
        auto operator()(std::string const& s) const
                -> basic_state<Rules, Board>
        {
                using set_type = set_t<Board>;
                set_type by_color_piece[2][2];
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

                for (auto const sq : Board::squares) {
                        sstr >> ch;
                        if (std::islower(ch) && ch == std::tolower(Token::black)) {
                                by_color_piece[0][0].insert(sq);
                        } else if (std::isupper(ch) && ch == std::toupper(Token::black)) {
                                by_color_piece[0][1].insert(sq);
                        } else if (std::islower(ch) && ch == std::tolower(Token::white)) {
                                by_color_piece[1][0].insert(sq);
                        } else if (std::isupper(ch) && ch == std::toupper(Token::white)) {
                                by_color_piece[1][1].insert(sq);
                        } else if (ch != Token::empty) {
                                assert(false);
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
                using board_type = board_t<State>;

                std::stringstream sstr;
                sstr << write_color<Token>(s.to_move());
                sstr << string<board_type>{}([&](auto const n) { return content<Token>(s, n); });
                sstr << '\n';
                return sstr.str();
        }
};

}       // inline namespace v1
}       // namespace dxp
}       // namespace dctl::core
