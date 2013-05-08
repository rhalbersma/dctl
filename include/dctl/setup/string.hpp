#pragma once
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

template<typename Token>
bool read_color(char c)
{
        switch(c) {
        case Token::black:
                return Side::black;
        case Token::white:
                return Side::white;
        default:
                BOOST_ASSERT(!"switch statement incomplete");
                return false;
        }
}

template<typename Token>
char write_color(bool color)
{
        return Token::color[color];
}

template
<
        typename Rules,
        typename Board,
        typename Protocol,
        typename Content = typename Token<Protocol>::type
>
struct read;

template
<
        typename Protocol,
        typename Content = typename Token<Protocol>::type
>
struct write;

template<typename Rules, typename Board, typename Token>
struct read<Rules, Board, pdn::protocol, Token>
{
        Position<Rules, Board> operator()(std::string const& s) const
        {
                BitBoard p_pieces[2] = {0, 0};
                BitBoard p_kings = 0;
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
                        switch(ch) {
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
                                        BOOST_ASSERT(Board::is_valid(sq - 1));
                                        auto b = Board::square2bit(sq - 1);     // convert square to bit
                                        auto bb = bit::singlet<BitBoard>(b);    // create bitboard
                                        p_pieces[setup_color] ^= bb;
                                        if (setup_kings)
                                                p_kings ^= bb;
                                }
                                setup_kings = false;
                                break;
                        }
                }
                return { p_pieces[Side::black], p_pieces[Side::white], p_kings, p_side };
        }
};

template<typename Token>
struct write<pdn::protocol, Token>
{
        template<typename Position>
        std::string operator()(Position const& p) const
        {
                typedef typename Position::board_type Board;

                std::stringstream sstr;
                sstr << Token::quote;                                           // opening quotes
                sstr << write_color<Token>(active_color(p));                   // side to move

                for (auto i = 0; i < 2; ++i) {
                        auto c = i != 0;
                        if (p.material().pieces(c)) {
                                sstr << Token::colon;                           // colon
                                sstr << Token::color[c];                        // color tag
                        }
                        for (auto bb = p.material().pieces(c); bb; bit::first::clear(bb)) {
                                if (bit::is_element(bit::first::equal(bb), p.material().kings()))
                                        sstr << Token::king;                    // king tag
                                auto b = bit::first::find(bb);                  // bit index
                                sstr << Board::bit2square(b) + 1;               // square number
                                if (bit::is_multiple(bb))                       // still pieces remaining
                                        sstr << Token::comma;                   // comma separator
                        }
                }
                sstr << Token::quote << "\n";                                   // closing quotes
                return sstr.str();
        }
};

template<typename Rules, typename Board, typename Token>
struct read<Rules, Board, dxp::protocol, Token>
{
        Position<Rules, Board> operator()(std::string const& s) const
        {
                BitBoard p_pieces[2] = {0, 0};
                BitBoard p_kings = 0;
                bool p_side = Side::black;

                std::stringstream sstr(s);
                char ch;
                sstr >> ch;
                p_side = read_color<Token>(ch);

                for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
                        auto b = Board::square2bit(sq);         // convert square to bit
                        auto bb = bit::singlet<BitBoard>(b);    // create bitboard
                        sstr >> ch;
                        switch(toupper(ch)) {
                        case Token::black:
                                p_pieces[Side::black] ^= bb;    // black piece
                                break;
                        case Token::white:
                                p_pieces[Side::white] ^= bb;    // white piece
                                break;
                        case Token::empty:
                                break;
                        default:
                                BOOST_ASSERT(!"switch statement incomplete");
                                break;
                        }
                        if (isupper(ch))
                                p_kings ^= bb;                  // king
                }
                return { p_pieces[Side::black], p_pieces[Side::white], p_kings, p_side };
        }
};

template<typename Token>
struct write<dxp::protocol, Token>
{
        template<typename Rules, typename Board>
        std::string operator()(Position<Rules, Board> const& p) const
        {
                std::stringstream sstr;
                sstr << write_color<Token>(active_color(p));                // side to move
                for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
                        auto b = Board::square2bit(sq);                 // convert square to bit
                        sstr << content<Token>(p.material(), b);        // bit content
                }
                sstr << "\n";
                return sstr.str();
        }
};

}       // namespace setup
}       // namespace dctl
