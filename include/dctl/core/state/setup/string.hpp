#pragma once
#include <dctl/core/state/color_piece.hpp>
#include <dctl/core/state/setup/diagram.hpp>
#include <dctl/core/state/setup/protocols.hpp>
#include <dctl/core/state/setup/i_token.hpp>
#include <dctl/core/state/state.hpp>
#include <dctl/util/type_traits.hpp>    // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <cassert>                      // assert
#include <cctype>                       // isdigit
#include <sstream>                      // stringstream
#include <string>                       // string

namespace dctl::core {
namespace setup {

template<class Token>
auto read_color(char c)
{
        switch (c) {
        case Token::black : return color::black;
        case Token::white : return color::white;
        default           : assert(false); return color::black;
        }
}

template<class Token>
char write_color(color const c)
{
        return Token::color[xstd::to_underlying_type(c)];
}

template
<
        class Rules,
        class Board,
        class Protocol,
        class Content = typename Token<Protocol>::type
>
struct read;

template
<
        class Protocol,
        class Content = typename Token<Protocol>::type
>
struct write;

template<class Rules, class Board, class Token>
struct read<Rules, Board, pdn::protocol, Token>
{
        state<Rules, Board> operator()(std::string const& s) const
        {
                using set_type = set_t<Board>;
                set_type by_color_piece[2][2]{};
                auto p_side = color::black;

                assert(by_color_piece[0][0].empty());
                assert(by_color_piece[0][1].empty());
                assert(by_color_piece[1][0].empty());
                assert(by_color_piece[1][1].empty());

                // do not attempt to parse empty strings
                if (s.empty())
                        return {{by_color_piece[0][0], by_color_piece[1][0], by_color_piece[0][1], by_color_piece[1][1]}, p_side };

                auto setup_color = p_side;
                auto setup_piece = piece::pawns;

                std::stringstream sstr(s);
                char ch;
                int sq;

                for (sstr >> ch; sstr; sstr >> ch) {
                        switch (ch) {
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
                                if (isdigit(ch)) {
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

template<class Token>
struct write<pdn::protocol, Token>
{
        template<class State>
        std::string operator()(State const& s) const
        {
                using Board = board_t<State>;

                std::stringstream sstr;
                sstr << Token::quote;                                   // opening quotes
                sstr << write_color<Token>(s.to_move());                // side to move

                for (auto i = 0; i < 2; ++i) {
                        auto c = i ? color::white : color::black;
                        if (!s.pieces(c).empty()) {
                                sstr << Token::colon;                   // colon
                                sstr << Token::color[xstd::to_underlying_type(c)];                // color tag
                        }
                        auto const bs = s.pieces(c);
                        auto n = 0;
                        for (auto sq : bs) {
                                if (s.pieces(kings_c).test(sq))
                                        sstr << Token::king;            // king tag
                                sstr << Board::square_from_bit(sq) + 1; // square number
                                //if (p.is_counted(c) && p.index(c) == sq)
                                //        sstr << "^" << p.count(c);
                                if (++n != bs.size())                  // still pieces remaining
                                        sstr << Token::comma;           // comma separator
                        }
                }
                sstr << Token::quote << '\n';                           // closing quotes
                return sstr.str();
        }
};

template<class Rules, class Board, class Token>
struct read<Rules, Board, dxp::protocol, Token>
{
        state<Rules, Board> operator()(std::string const& s) const
        {
                using set_type = set_t<Board>;
                set_type by_color_piece[2][2]{};
                auto p_side = color::black;

                assert(by_color_piece[0][0].empty());
                assert(by_color_piece[0][1].empty());
                assert(by_color_piece[1][0].empty());
                assert(by_color_piece[1][1].empty());

                // do not attempt to parse empty strings
                if (s.empty())
                        return {{by_color_piece[0][0], by_color_piece[1][0], by_color_piece[0][1], by_color_piece[1][1]}, p_side };

                std::stringstream sstr(s);
                char ch;
                sstr >> ch;
                p_side = read_color<Token>(ch);

                 for (auto sq = 0; sq < Board::size(); ++sq) {
                        auto b = Board::bit_from_square(sq);
                        sstr >> ch;
                        switch (toupper(ch)) {
                        case Token::black : by_color_piece[xstd::to_underlying_type(color::black)][isupper(ch)].insert(b); break;
                        case Token::white : by_color_piece[xstd::to_underlying_type(color::white)][isupper(ch)].insert(b); break;
                        case Token::empty : break;
                        default           : assert(false);
                        }
                }
                return {{by_color_piece[0][0], by_color_piece[1][0], by_color_piece[0][1], by_color_piece[1][1]}, p_side };
        }
};

template<class Token>
struct write<dxp::protocol, Token>
{
        template<class Rules, class Board>
        std::string operator()(state<Rules, Board> const& p) const
        {
                std::stringstream sstr;
                sstr << write_color<Token>(p.to_move());    // side to move
                for (auto sq = 0; sq < Board::size(); ++sq) {
                        auto b = Board::bit_from_square(sq);    // convert square to bit
                        sstr << content<Token>(p, b);           // bit content
                }
                sstr << '\n';
                return sstr.str();
        }
};

}       // namespace setup
}       // namespace dctl::core
