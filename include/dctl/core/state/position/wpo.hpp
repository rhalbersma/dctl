#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color_piece.hpp>      // color, black, white, piece, pawns, kings, occup, empty
#include <dctl/core/state/position/legal.hpp>   // is_legal
#include <dctl/util/type_traits.hpp>            // set_t
#include <hash_append/hash_append.h>            // hash_append
#include <tuple>                                // tie
#include <type_traits>                          // is_pod

namespace dctl::core {
namespace wpo {

template<class Board>
class position
{
        constexpr static auto static_assert_type_traits() noexcept
        {
                static_assert(std::is_pod_v<position>);
        }

        set_t<Board> m_white;
        set_t<Board> m_pawns;
        set_t<Board> m_occup;
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

        position() = default;

        constexpr position(set_type const black_pawns, set_type const white_pawns, set_type const black_kings, set_type const white_kings) // Throws: Nothing.
        :
                m_white{white_pawns | white_kings},
                m_pawns{black_pawns | white_pawns},
                m_occup{m_white | m_pawns | black_kings}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        constexpr position(set_type const black_pawns, set_type const white_pawns) // Throws: Nothing.
        :
                m_white{white_pawns},
                m_pawns{black_pawns | white_pawns},
                m_occup{m_pawns}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        template<class ColorT, class Action, std::enable_if_t<
                is_color_v<ColorT>
        >...>
        constexpr auto make(ColorT const c, Action const& a) // Throws: Nothing.
        {
                if (a.is_jump()) {
                        m_white -= a.captured_pieces();
                        m_pawns -= a.captured_pieces();
                        m_occup ^= a.captured_pieces();
                }

                m_occup
                        .erase(a.from())
                        .insert(a.dest())
                ;
                if constexpr (std::is_same_v<ColorT, color>) {
                        if (c == color::white) {
                                m_white
                                        .erase(a.from())
                                        .insert(a.dest())
                                ;
                        }
                } else if constexpr (std::is_same_v<ColorT, white_>) {
                        m_white
                                .erase(a.from())
                                .insert(a.dest())
                        ;
                }
                if (a.with() == piece::pawns) {
                        m_pawns.erase(a.from());
                }
                if (a.into() == piece::pawns) {
                        m_pawns.insert(a.dest());
                }
        }

        template<class ColorT, std::enable_if_t<
                is_color_v<ColorT>
        >...>
        constexpr auto pieces(ColorT const c) const noexcept
        {
                if constexpr (std::is_same_v<ColorT, color>) {
                        return c == color::black ? pieces(black_c) : pieces(white_c);
                } else {
                        if constexpr (c == black_c) { return m_white ^ m_occup; }
                        if constexpr (c == white_c) { return m_white;           }
                }
        }

        template<class PieceT, std::enable_if_t<
                is_piece_v<PieceT>
        >...>
        constexpr auto pieces(PieceT const p) const noexcept
        {
                if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawns ? pieces(pawns_c) : pieces(kings_c);
                } else {
                        if constexpr (p == pawns_c) { return m_pawns;           }
                        if constexpr (p == kings_c) { return m_pawns ^ m_occup; }
                }
        }

        template<class ColorT, class PieceT, std::enable_if_t<
                is_color_v<ColorT> &&
                is_piece_v<PieceT>
        >...>
        constexpr auto pieces(ColorT const c, PieceT const p) const noexcept
        {
                if constexpr (std::is_same_v<ColorT, color> && std::is_same_v<PieceT, piece>) {
                        return c == color::black ?
                                (p == piece::pawns ? pieces(black_c, pawns_c) : pieces(black_c, kings_c)) :
                                (p == piece::pawns ? pieces(white_c, pawns_c) : pieces(white_c, kings_c))
                        ;
                } else if constexpr (std::is_same_v<ColorT, color>) {
                        return c == color::black ? pieces(black_c, p) : pieces(white_c, p);
                } else if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawns ? pieces(c, pawns_c) : pieces(c, kings_c);
                } else {
                        if constexpr (c == black_c && p == pawns_c) { return ~m_white &  m_pawns;            }
                        if constexpr (c == black_c && p == kings_c) { return (m_white |  m_pawns) ^ m_occup; }
                        if constexpr (c == white_c && p == pawns_c) { return  m_white &  m_pawns;            }
                        if constexpr (c == white_c && p == kings_c) { return  m_white & ~m_pawns;            }
                }
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return m_occup;
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return board_type::squares() ^ m_occup;
        }

        template<class... Args>
        auto num_pieces(Args&&... args) const noexcept
        {
                static_assert(sizeof...(Args) <= 2);
                return pieces(std::forward<Args>(args)...).size();
        }

        constexpr auto tied() const noexcept
        {
                return std::tie(m_white, m_pawns, m_occup);
        }

        template<class HashAlgorithm>
        friend auto hash_append(HashAlgorithm& h, position const& p)
        {
                using xstd::hash_append;
                hash_append(h, p.m_white, p.m_pawns, p.m_occup);
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

}       // namespace wpo
}       // namespace dctl::core
