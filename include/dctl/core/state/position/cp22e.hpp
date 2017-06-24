#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color_piece.hpp>      // color, black, white, piece, pawns, kings, occup, empty
#include <dctl/core/state/position/legal.hpp>   // is_legal
#include <dctl/util/type_traits.hpp>            // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <tuple>                                // tie
#include <type_traits>                          // is_pod

namespace dctl::core {
namespace cp22e {

template<class Board>
class position
{
        constexpr static auto static_assert_type_traits() noexcept
        {
                static_assert(std::is_pod_v<position>);
        }

        std::array<std::array<set_t<Board>, 2>, 2> m_color_piece;
        set_t<Board> m_empty;
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

        position() = default;

        constexpr position(set_type const black_pawns, set_type const white_pawns, set_type const black_kings, set_type const white_kings) // Throws: Nothing.
        :
                m_color_piece{{ {{ black_pawns, black_kings }}, {{ white_pawns, white_kings }} }},
                m_empty{board_type::squares ^ (black_pawns | white_pawns | black_kings | white_kings)}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        constexpr position(set_type const black_pawns, set_type const white_pawns) // Throws: Nothing.
        :
                m_color_piece{{ {{ black_pawns, {} }}, {{ white_pawns, {} }} }},
                m_empty{board_type::squares ^ (black_pawns | white_pawns)}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        template<class Action>
        constexpr auto make(color const c, Action const& a) // Throws: Nothing.
        {
                if (a.is_jump()) {
                        set_pieces(not c, pawns_c) -= a.captured_pieces();
                        set_pieces(not c, kings_c) -= a.captured_pieces();
                        m_empty ^= a.captured_pieces();
                }

                set_pieces(c, a.with()).erase(a.from());
                set_pieces(c, a.into()).insert(a.dest());
                m_empty
                        .insert(a.from())
                        .erase(a.dest())
                ;
        }

        constexpr auto pieces(color const c) const noexcept
        {
                return pieces(c, pawns_c) | pieces(c, kings_c);
        }

        constexpr auto pieces(piece const p) const noexcept
        {
                return pieces(black_c, p) | pieces(white_c, p);
        }

        constexpr auto pieces(color const c, piece const p) const noexcept
        {
                return m_color_piece[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return board_type::squares ^ m_empty;
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return m_empty;
        }

        template<class... Args>
        auto num_pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return pieces(std::forward<Args>(args)...).size();
        }

        constexpr auto tied() const noexcept
        {
                return std::tie(m_color_piece[0][0], m_color_piece[1][0], m_color_piece[0][1], m_color_piece[1][1]);
        }

        template<class HashAlgorithm>
        friend auto hash_append(HashAlgorithm& h, position const& p)
        {
                using xstd::hash_append;
                hash_append(h, p.m_color_piece[0][0], p.m_color_piece[1][0], p.m_color_piece[0][1], p.m_color_piece[1][1]);
        }

private:
        constexpr auto& set_pieces(color const c, piece const p) noexcept
        {
                return m_color_piece[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }
};

template<class Board>
constexpr auto operator==(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return lhs.tied() == rhs.tied();
}

template<class Board>
constexpr auto operator< (position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return lhs.tied() < rhs.tied();
}

template<class Board>
constexpr auto operator!=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return not (lhs == rhs);
}

template<class Board>
constexpr auto operator> (position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Board>
constexpr auto operator>=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return not (lhs < rhs);
}

template<class Board>
constexpr auto operator<=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return not (rhs < lhs);
}

}       // namespace cp22e
}       // namespace dctl::core
