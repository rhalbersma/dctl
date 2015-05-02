#pragma once
#include <dctl/position/position.hpp>
#include <dctl/color.hpp>
#include <dctl/setup/diagram.hpp>
#include <dctl/setup/protocols.hpp>
#include <dctl/setup/i_token.hpp>
#include <dctl/set_type.hpp>
#include <dctl/type_traits.hpp>
#include <cassert>                      // assert
#include <cctype>                       // isdigit
#include <sstream>                      // stringstream
#include <string>                       // string

namespace dctl {
namespace setup {

template<class Token>
auto read_color(char c)
{
        switch (c) {
        case Token::black : return Color::black;
        case Token::white : return Color::white;
        default           : assert(false); return Color::black;
        }
}

template<class Token>
char write_color(Color c)
{
        return Token::color[static_cast<bool>(c)];
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
        Position<Rules, Board> operator()(std::string const& s) const
        {
                using Set = set_type<Board>;
                Set by_color[2]{};
                Set by_piece[2]{};
                auto p_side = Color::black;

                // do not attempt to parse empty strings
                if (s.empty())
                        return { by_color[0], by_color[1], by_piece[0], by_piece[1], p_side };

                auto setup_color = p_side;
                auto setup_piece = Piece::pawn;

                std::stringstream sstr(s);
                char ch;
                std::size_t sq;

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
                                setup_piece = Piece::king;
                                break;
                        default:
                                if (isdigit(ch)) {
                                        sstr.putback(ch);
                                        sstr >> sq;                             // read square
                                        //assert(Board::is_valid(sq - 1));
                                        auto b = Board::bit_from_square(sq - 1);     // convert square to bit
                                        by_color[static_cast<std::size_t>(setup_color)].set(b);
                                        by_piece[static_cast<std::size_t>(setup_piece)].set(b);
                                }
                                setup_piece = Piece::pawn;
                                break;
                        }
                }
                return { by_color[0], by_color[1], by_piece[0], by_piece[1], p_side };
        }
};

template<class Token>
struct write<pdn::protocol, Token>
{
        template<class Position>
        std::string operator()(Position const& p) const
        {
                using Board = board_type_t<Position>;

                std::stringstream sstr;
                sstr << Token::quote;                                   // opening quotes
                sstr << write_color<Token>(p.to_move());                // side to move

                for (auto i = 0; i < 2; ++i) {
                        auto c = i ? Color::white : Color::black;
                        if (p.pieces(c).any()) {
                                sstr << Token::colon;                   // colon
                                sstr << Token::color[static_cast<bool>(c)];                // color tag
                        }
                        auto const bs = p.pieces(c);
                        std::size_t n = 0;
                        for (auto&& sq : bs) {
                                if (p.pieces(Piece::king).test(sq))
                                        sstr << Token::king;            // king tag
                                sstr << Board::square_from_bit(sq) + 1; // square number
                                //if (p.is_counted(c) && p.index(c) == sq)
                                //        sstr << "^" << p.count(c);
                                if (++n != bs.count())                  // still pieces remaining
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
        Position<Rules, Board> operator()(std::string const& s) const
        {
                using Set = set_type<Board>;
                Set by_color[2]{ Set{}, Set{} };
                Set by_piece[2]{ Set{}, Set{} };
                auto p_side = Color::black;

                std::stringstream sstr(s);
                char ch;
                sstr >> ch;
                p_side = read_color<Token>(ch);

                 for (auto&& sq : Board::squares()) {
                        auto b = Board::bit_from_square(sq);
                        sstr >> ch;
                        switch (toupper(ch)) {
                        case Token::black : by_color[static_cast<bool>(Color::black)].set(b); break;
                        case Token::white : by_color[static_cast<bool>(Color::white)].set(b); break;
                        case Token::empty : break;
                        default           : assert(false);
                        }
                        by_piece[isupper(ch)].set(b);                  // king
                }
                return { by_color[0], by_color[1], by_piece[0], by_piece[1], p_side };
        }
};

template<class Token>
struct write<dxp::protocol, Token>
{
        template<class Rules, class Board>
        std::string operator()(Position<Rules, Board> const& p) const
        {
                std::stringstream sstr;
                sstr << write_color<Token>(p.to_move());    // side to move
                for (auto&& sq : Board::squares()) {
                        auto b = Board::bit_from_square(sq);    // convert square to bit
                        sstr << content<Token>(p, b);           // bit content
                }
                sstr << '\n';
                return sstr.str();
        }
};

}       // namespace setup
}       // namespace dctl
