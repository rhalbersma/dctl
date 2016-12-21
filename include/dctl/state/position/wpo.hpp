#pragma once
#include <dctl/board_traits.hpp>                // squares
#include <dctl/color_piece.hpp>                 // color, black, white, piece, pawns, kings, occup, empty
#include <dctl/state/position/legal.hpp>        // is_legal
#include <dctl/utility/type_traits.hpp>         // set_t
#include <hash_append/hash_append.h>            // hash_append
#include <tuple>                                // tie
#include <type_traits>                          // is_pod

namespace dctl {
namespace wpo {

template<class Board>
class position
{
        static constexpr auto static_assert_type_traits() noexcept
        {
                static_assert(std::is_pod<position>{});
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
                m_occup{black_pawns | white_pawns | black_kings | white_kings}
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

        template<class Action>
        constexpr auto make(color const c, Action const& a) // Throws: Nothing.
        {
                m_occup
                        .erase(a.from())
                        .insert(a.dest())
                ;

                if (c == white_c) {
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

                if (a.is_jump()) {
                        m_white -= a.captured_pieces();
                        m_pawns -= a.captured_pieces();
                        m_occup ^= a.captured_pieces();
                }
        }

        template<color Side>
        constexpr auto pieces(color_<Side>) const noexcept
        {
                if constexpr (Side == color::black) { return m_white ^ m_occup; }
                if constexpr (Side == color::white) { return m_white;           }
        }

        constexpr auto pieces(color const c) const noexcept
        {
                return c == color::black ? pieces(black_c) : pieces(white_c);
        }

        template<piece Type>
        constexpr auto pieces(piece_<Type>) const noexcept
        {
                if constexpr (Type == piece::pawns) { return m_pawns;           }
                if constexpr (Type == piece::kings) { return m_pawns ^ m_occup; }
        }

        constexpr auto pieces(piece const p) const noexcept
        {
                return p == piece::pawns ? pieces(pawns_c) : pieces(kings_c);
        }

        template<color Side, piece Type>
        constexpr auto pieces(color_<Side>, piece_<Type>) const noexcept
        {
                if constexpr (Side == color::black && Type == piece::pawns) { return ~m_white &  m_pawns;            }
                if constexpr (Side == color::black && Type == piece::kings) { return (m_white |  m_pawns) ^ m_occup; }
                if constexpr (Side == color::white && Type == piece::pawns) { return  m_white &  m_pawns;            }
                if constexpr (Side == color::white && Type == piece::kings) { return  m_white & ~m_pawns;            }
        }

        template<piece Type>
        constexpr auto pieces(color const c, piece_<Type> const p) const noexcept
        {
                return c == color::black ? pieces(black_c, p) : pieces(white_c, p);
        }

        template<color Side>
        constexpr auto pieces(color_<Side> const c, piece const p) const noexcept
        {
                return p == piece::pawns ? pieces(c, pawns_c) : pieces(c, kings_c);
        }

        constexpr auto pieces(color const c, piece const p) const noexcept
        {
                return pieces(
                        c == color::black ? black_c : white_c,
                        p == piece::pawns ? pawns_c : kings_c
                );
        }

        constexpr auto pieces(occup_) const noexcept
        {
                return m_occup;
        }

        constexpr auto pieces(empty_) const noexcept
        {
                return squares_v<board_type> ^ m_occup;
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

}       // namespace wpo
}       // namespace dctl
