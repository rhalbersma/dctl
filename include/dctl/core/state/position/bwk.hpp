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
namespace bwk {

template<class Board>
class position
{
        constexpr static auto static_assert_type_traits() noexcept
        {
                static_assert(std::is_pod_v<position>);
        }

        std::array<set_t<Board>, 2> m_color;
        set_t<Board> m_kings;
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

        position() = default;

        constexpr position(set_type const black_pawns, set_type const white_pawns, set_type const black_kings, set_type const white_kings) // Throws: Nothing.
        :
                m_color{{ black_pawns | black_kings, white_pawns | white_kings }},
                m_kings{black_kings | white_kings}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        constexpr position(set_type const black_pawns, set_type const white_pawns) // Throws: Nothing.
        :
                m_color{{ black_pawns, white_pawns }},
                m_kings{}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        template<class Action>
        constexpr auto make(color const c, Action const& a) // Throws: Nothing.
        {
                if (a.is_jump()) {
                        set_pieces(!c) ^= a.captured_pieces();
                        m_kings -= a.captured_pieces();
                }

                set_pieces(c)
                        .erase(a.from())
                        .insert(a.dest())
                ;
                if (a.with() == piece::kings) {
                        m_kings
                                .erase(a.from())
                                .insert(a.dest())
                        ;
                } else if (a.into() == piece::kings) {
                        m_kings.insert(a.dest());
                }
        }

        constexpr auto pieces(color const c) const noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
        }

        template<class PieceT, std::enable_if_t<
                is_piece_v<PieceT>
        >...>
        constexpr auto pieces(PieceT const p) const noexcept
        {
                if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawns ? pieces(pawns_c) : pieces(kings_c);
                } else {
                        if constexpr (p == pawns_c) { return m_kings ^ pieces(occup_c); }
                        if constexpr (p == kings_c) { return m_kings;                   }
                }
        }

        template<class PieceT, std::enable_if_t<
                is_piece_v<PieceT>
        >...>
        constexpr auto pieces(color const c, PieceT const p) const noexcept
        {
                if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawns ? pieces(c, pawns_c) : pieces(c, kings_c);
                } else {
                        if constexpr (p == pawns_c) { return pieces(c) & ~m_kings; }
                        if constexpr (p == kings_c) { return pieces(c) &  m_kings; }
                }
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return pieces(black_c) | pieces(white_c);
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return board_type::squares ^ pieces(occup_c);
        }

        template<class... Args>
        auto num_pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return pieces(std::forward<Args>(args)...).count();
        }

        constexpr auto tied() const noexcept
        {
                return std::tie(m_color[0], m_color[1], m_kings);
        }

        template<class HashAlgorithm>
        friend auto hash_append(HashAlgorithm& h, position const& p)
        {
                using xstd::hash_append;
                hash_append(h, p.m_color, p.m_kings);
        }

private:
        constexpr auto& set_pieces(color const c) noexcept
        {
                return m_color[xstd::to_underlying_type(c)];
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
        return !(lhs == rhs);
}

template<class Board>
constexpr auto operator> (position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class Board>
constexpr auto operator>=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class Board>
constexpr auto operator<=(position<Board> const& lhs, position<Board> const& rhs) noexcept
{
        return !(rhs < lhs);
}

}       // namespace bwk
}       // namespace dctl::core
