#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/basic_board.hpp>
#include <dctl/core/board/string.hpp>
#include <dctl/core/state/basic_state.hpp>
#include <dctl/core/state/color_piece.hpp>
#include <dctl/core/ui/basic_token_set.hpp>     // basic_token_set
#include <dctl/core/ui/color.hpp>               // read_color, write_color
#include <dctl/util/type_traits.hpp>            // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert
#include <cctype>                               // isdigit
#include <sstream>                              // stringstream
#include <string>                               // string

namespace dctl::core {
namespace pdn {

struct token_set
:
        public basic_token_set<'B', 'W', '.'>
{
        constexpr static auto king  = 'K';
        constexpr static auto colon = ':';
        constexpr static auto comma = ',';
};

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
                        return {{by_color_piece[0][0], by_color_piece[1][0], by_color_piece[0][1], by_color_piece[1][1]}, p_side };
                }

                auto setup_color = p_side;
                auto setup_piece = piece::pawns;

                std::stringstream sstr(s);
                char ch;
                int sq;

                for (sstr >> ch; sstr; sstr >> ch) {
                        switch (std::toupper(ch)) {
                        case Token::black :
                        case Token::white :
                                p_side = read_color<Token>(ch);
                                break;
                        case Token::colon :
                                sstr >> ch;
                                setup_color = read_color<Token>(ch);
                                break;
                        case Token::king :                                      // setup kings
                                setup_piece = piece::kings;
                                break;
                        default:
                                if (std::isdigit(ch)) {
                                        sstr.putback(ch);
                                        sstr >> sq;                             // read square
                                        //assert(Board::is_valid(sq - 1));
                                        auto b = Board::bit_from_square(sq - 1);     // convert square to bit
                                        by_color_piece[xstd::to_underlying_type(setup_color)][xstd::to_underlying_type(setup_piece)].insert(b);
                                }
                                setup_piece = piece::pawns;
                                break;
                        }
                }
                return {{by_color_piece[0][0], by_color_piece[1][0], by_color_piece[0][1], by_color_piece[1][1]}, p_side };
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
                sstr << write_color<Token>(s.to_move());                                // side to move
                for (auto i = 0; i < 2; ++i) {
                        auto c = i ? color::white : color::black;
                        if (!s.pieces(c).empty()) {
                                sstr << Token::colon;                                   // colon
                                sstr << Token::color[xstd::to_underlying_type(c)];      // color tag
                        }
                        auto const bs = s.pieces(c);
                        auto n = 0;
                        for (auto const sq : bs) {
                                if (s.pieces(kings_c).contains(sq)) {
                                        sstr << Token::king;                            // king tag
                                }
                                sstr << board_type::square_from_bit(sq) + 1;            // square number
                                //if (p.is_counted(c) && p.index(c) == sq)
                                //        sstr << "^" << p.count(c);
                                if (++n != bs.size()) {                                 // still pieces remaining
                                        sstr << Token::comma;                           // comma separator
                                }
                        }
                }
                return sstr.str();
        }
};

}       // namespace pdn
}       // namespace dctl::core
