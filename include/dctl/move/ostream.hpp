#pragma once
#include <cassert>
#include <iosfwd>                       // ostream
#include <sstream>                      // stringstream
#include <type_traits>
#include <dctl/move/move.hpp>           // Move
#include <dctl/board/types.hpp>
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace format {

template<class Move>
struct traits
:
        std::integral_constant<int, 1>
{};

template<class Board>
struct traits<Move<rules::Russian, Board>>
:
        std::integral_constant<int, 2>
{};

template<class Board>
struct traits<Move<rules::Czech, Board>>
:
        std::integral_constant<int, 2>
{};

template<>
struct traits<Move<rules::International, board::Checkers>>
:
        std::integral_constant<int, 2>
{};

inline
auto index()
{
        static auto const slot = std::ios_base::xalloc();
        return slot;
}

template<class CharT, class Traits>
auto& numeric(std::basic_ostream<CharT, Traits>& ostr)
{
        ostr.iword(index()) = 1;
        return ostr;
}

template<class CharT, class Traits>
auto& algebraic(std::basic_ostream<CharT, Traits>& ostr)
{
        ostr.iword(index()) = 2;
        return ostr;
}

template<class Move>
auto as_numeric(Move const& m)
{
        using Board = typename Move::board_type;
        std::stringstream sstr;
        sstr << Board::numeric_from_bit(m.from());
        sstr << (m.is_jump() ? 'x' : '-');
        sstr << Board::numeric_from_bit(m.dest());
        return sstr.str();
}

template<class Move>
auto as_algebraic(Move const& m)
{
        using Board = typename Move::board_type;
        std::stringstream sstr;
        sstr << Board::algebraic_from_bit(m.from());
        sstr << (m.is_jump() ? 'x' : '-');
        sstr << Board::algebraic_from_bit(m.dest());
        return sstr.str();
}

}       // namespace format

template<class Rules, class Board>
auto& operator<<(std::ostream& ostr, Move<Rules, Board> const& m)
{
        auto value = ostr.iword(format::index());
        if (!value)
                value = format::traits<Move<Rules, Board>>::value;
        switch(value) {
        case 1:
                ostr << format::as_numeric(m);
                break;
        case 2:
                ostr << format::as_algebraic(m);
                break;
        default:
                assert(false && !"Supplied move format not supported.");
                break;
        }
        return ostr;
}

}       // namespace dctl
