#pragma once
#include <dctl/core/board_traits.hpp>   // squares
#include <dctl/core/color_piece.hpp>    // color, black, white, piece, pawns, kings, occup, empty
#include <dctl/core/position/legal.hpp> // is_legal
#include <dctl/util/type_traits.hpp>    // set_t
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <array>                        // array
#include <tuple>                        // tie
#include <type_traits>                  // is_pod

namespace dctl {
namespace bwke {

template<class Board>
class position
{
        static constexpr auto static_assert_type_traits() noexcept
        {
                static_assert(std::is_pod_v<position>);
        }

        std::array<set_t<Board>, 2> m_color;
        set_t<Board> m_kings;
        set_t<Board> m_empty;
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

        position() = default;

        constexpr position(set_type const black_pawns, set_type const white_pawns, set_type const black_kings, set_type const white_kings) // Throws: Nothing.
        :
                m_color{black_pawns | black_kings, white_pawns | white_kings},
                m_kings{black_kings | white_kings},
                m_empty{squares_v<board_type> ^ (m_color[0] | m_color[1])}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        constexpr position(set_type const black_pawns, set_type const white_pawns) // Throws: Nothing.
        :
                m_color{black_pawns, white_pawns},
                m_kings{},
                m_empty{squares_v<board_type> ^ (m_color[0] | m_color[1])}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        template<class ColorT, class Action, std::enable_if_t<
                is_color_v<ColorT>
        >...>
        constexpr auto make(ColorT const c, Action const& a) // Throws: Nothing.
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

                m_empty = squares_v<board_type> ^ (pieces(black_c) | pieces(white_c));
        }

        template<class ColorT, std::enable_if_t<
                is_color_v<ColorT>
        >...>
        constexpr auto pieces(ColorT const c) const noexcept
        {
                if constexpr (std::is_same_v<ColorT, color>) {
                        return m_color[xstd::to_underlying_type(c)];
                } else {
                        return std::get<xstd::to_underlying_type(c)>(m_color);
                }
        }

        template<class PieceT, std::enable_if_t<
                is_piece_v<PieceT>
        >...>
        constexpr auto pieces([[maybe_unused]] PieceT const p) const noexcept
        {
                if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawns ? pieces(pawns_c) : pieces(kings_c);
                } else {
                        if constexpr (PieceT::value == piece::pawns) { return m_kings ^ pieces(occup_c); }
                        if constexpr (PieceT::value == piece::kings) { return m_kings;                   }
                }
        }

        template<class ColorT, class PieceT, std::enable_if_t<
                is_color_v<ColorT> &&
                is_piece_v<PieceT>
        >...>
        constexpr auto pieces(ColorT const c, [[maybe_unused]] PieceT const p) const noexcept
        {
                if constexpr (std::is_same_v<PieceT, piece>) {
                        return p == piece::pawns ? pieces(c, pawns_c) : pieces(c, kings_c);
                } else {
                        if constexpr (PieceT::value == piece::pawns) { return pieces(c) & ~m_kings; }
                        if constexpr (PieceT::value == piece::kings) { return pieces(c) &  m_kings; }
                }
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return squares_v<board_type> ^ m_empty;
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
                return std::tie(m_color[0], m_color[1], m_kings);
        }

        template<class HashAlgorithm>
        friend auto hash_append(HashAlgorithm& h, position const& p)
        {
                using xstd::hash_append;
                hash_append(h, p.m_color, p.m_kings);
        }

private:
        template<class ColorT, std::enable_if_t<
                is_color_v<ColorT>
        >...>
        constexpr auto& set_pieces(ColorT const c) noexcept
        {
                if constexpr (std::is_same_v<ColorT, color>) {
                        return m_color[xstd::to_underlying_type(c)];
                } else {
                        return std::get<xstd::to_underlying_type(c)>(m_color);
                }
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

}       // namespace bwke
}       // namespace dctl
