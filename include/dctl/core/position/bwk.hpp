#pragma once
#include <dctl/core/board_traits.hpp>                // squares
#include <dctl/core/color_piece.hpp>                 // color, black, white, piece, pawns, kings, occup, empty
#include <dctl/core/position/legal.hpp>        // is_legal
#include <dctl/util/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <tuple>                                // tie
#include <type_traits>                          // is_pod

namespace dctl {
namespace bwk {

template<class Board>
class position
{
        static constexpr auto static_assert_type_traits() noexcept
        {
                static_assert(std::is_pod<position>{});
        }

        set_t<Board> m_color[2];
        set_t<Board> m_kings;
public:
        using board_type = Board;
        using   set_type = set_t<Board>;

        position() = default;

        constexpr position(set_type const black_pawns, set_type const white_pawns, set_type const black_kings, set_type const white_kings) // Throws: Nothing.
        :
                m_color{black_pawns | black_kings, white_pawns | white_kings},
                m_kings{black_kings | white_kings}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns, black_kings, white_kings));
        }

        constexpr position(set_type const black_pawns, set_type const white_pawns) // Throws: Nothing.
        :
                m_color{black_pawns, white_pawns},
                m_kings{}
        {
                assert(is_legal<board_type>(black_pawns, white_pawns));
        }

        template<class Action>
        constexpr auto make(color const c, Action const& a) // Throws: Nothing.
        {
                set_pieces(c)
                        .erase(a.from())
                        .insert(a.dest())
                ;

                if (a.is_jump()) {
                        set_pieces(!c) ^= a.captured_pieces();
                        m_kings &= ~a.captured_pieces();
                }

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

        template<piece Type>
        constexpr auto pieces(piece_<Type>) const noexcept
        {
                if constexpr (Type == piece::pawns) { return m_kings ^ pieces(occup_c); }
                if constexpr (Type == piece::kings) { return m_kings;                   }
        }

        constexpr auto pieces(piece const p) const noexcept
        {
                return p == pawns_c ? pieces(pawns_c) : pieces(kings_c);
        }

        template<piece Type>
        constexpr auto pieces(color const c, piece_<Type>) const noexcept
        {
                if constexpr (Type == piece::pawns) { return pieces(c) & ~m_kings; }
                if constexpr (Type == piece::kings) { return pieces(c) &  m_kings; }
        }

        constexpr auto pieces(color const c, piece const p) const noexcept
        {
                return p == pawns_c ? pieces(c, pawns_c) : pieces(c, kings_c);
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return pieces(black_c) | pieces(white_c);
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return squares_v<board_type> ^ pieces(occup_c);
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
}       // namespace dctl
