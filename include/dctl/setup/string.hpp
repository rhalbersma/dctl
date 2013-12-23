#pragma once
#include <cassert>                      // assert
#include <cctype>                       // isdigit
#include <sstream>                      // stringstream
#include <string>                       // string
#include <dctl/setup/diagram.hpp>
#include <dctl/setup/protocols.hpp>
#include <dctl/setup/i_token.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/node/position.hpp>

namespace dctl {
namespace setup {

template<class Token>
bool read_color(char c)
{
        switch (c) {
        case Token::black:
                return Side::black;
        case Token::white:
                return Side::white;
        default:
                assert(false);
                return false;
        }
}

template<class Token>
char write_color(bool color)
{
        return Token::color[color];
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
                using T = typename Board::set_type;
                T p_pieces[2] {};
                T p_kings {};
                bool p_side = Side::black;

                // do not attempt to parse empty strings
                if (s.empty())
                        return { p_pieces[Side::black], p_pieces[Side::white], p_kings, p_side };

                bool setup_kings = false;
                bool setup_color = p_side;

                std::stringstream sstr(s);
                char ch;
                int sq;

                for (sstr >> ch; sstr; sstr >> ch) {
                        switch (ch) {
                        case Token::black:
                        case Token::white:
                                p_side = read_color<Token>(ch);
                                break;
                        case Token::colon:
                                sstr >> ch;
                                setup_color = read_color<Token>(ch);
                                break;
                        case Token::king:                                       // setup kings
                                setup_kings = true;
                                break;
                        default:
                                if (isdigit(ch)) {
                                        sstr.putback(ch);
                                        sstr >> sq;                             // read square
                                        //assert(Board::is_valid(sq - 1));
                                        auto b = Board::bit_from_square(sq - 1);     // convert square to bit
                                        p_pieces[setup_color].set(b);
                                        if (setup_kings)
                                                p_kings.set(b);
                                }
                                setup_kings = false;
                                break;
                        }
                }
                return { p_pieces[Side::black], p_pieces[Side::white], p_kings, p_side };
        }
};

template<class Token>
struct write<pdn::protocol, Token>
{
        template<class Position>
        std::string operator()(Position const& p) const
        {
                using Board = typename Position::board_type;

                std::stringstream sstr;
                sstr << Token::quote;                                           // opening quotes
                sstr << write_color<Token>(active_color(p));                   // side to move

                for (auto i = 0; i < 2; ++i) {
                        auto c = i != 0;
                        if (!p.pieces(c).empty()) {
                                sstr << Token::colon;                           // colon
                                sstr << Token::color[c];                        // color tag
                        }
                        auto const bs = p.pieces(c);
                        auto n = 0;
                        for (auto sq : bs) {
                                if (p.kings().test(sq))
                                        sstr << Token::king;                    // king tag
                                sstr << Board::square_from_bit(sq) + 1;              // square number
                                if (++n != bs.size())                           // still pieces remaining
                                        sstr << Token::comma;                   // comma separator
                        }
                }
                sstr << Token::quote << '\n';                                   // closing quotes
                return sstr.str();
        }
};

template<class Rules, class Board, class Token>
struct read<Rules, Board, dxp::protocol, Token>
{
        Position<Rules, Board> operator()(std::string const& s) const
        {
                using T = typename Board::set_type;
                T p_pieces[2] {};
                T p_kings {};
                bool p_side = Side::black;

                std::stringstream sstr(s);
                char ch;
                sstr >> ch;
                p_side = read_color<Token>(ch);

                 for (auto sq : Board::squares()) {
                        auto b = Board::bit_from_square(sq);
                        sstr >> ch;
                        switch (toupper(ch)) {
                        case Token::black:
                                p_pieces[Side::black].set(b);    // black piece
                                break;
                        case Token::white:
                                p_pieces[Side::white].set(b);    // white piece
                                break;
                        case Token::empty:
                                break;
                        default:
                                assert(false);
                                break;
                        }
                        if (isupper(ch))
                                p_kings.set(b);                  // king
                }
                return { p_pieces[Side::black], p_pieces[Side::white], p_kings, p_side };
        }
};

template<class Token>
struct write<dxp::protocol, Token>
{
        template<class Rules, class Board>
        std::string operator()(Position<Rules, Board> const& p) const
        {
                std::stringstream sstr;
                sstr << write_color<Token>(active_color(p));                // side to move
                for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
                        auto b = Board::bit_from_square(sq);                 // convert square to bit
                        sstr << content<Token>(p, b);        // bit content
                }
                sstr << '\n';
                return sstr.str();
        }
};

}       // namespace setup
}       // namespace dctl
