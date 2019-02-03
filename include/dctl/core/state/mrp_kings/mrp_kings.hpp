#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/color.hpp>
#include <dctl/core/state/piece.hpp>
#include <dctl/core/rules/type_traits.hpp>
#include <dctl/util/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cstddef>

namespace dctl::core {

template<class Rules, class Board>
class MostRecentlyPushedking
{
public:
        enum { M = max_same_king_move_v<Rules> };
        enum { N = board::bits() };

private:
        std::size_t index_[2]{ N, N };
        std::size_t count_[2]{};
        std::size_t color_piece_[2][2];

public:
        template<class position>
        explicit constexpr MostRecentlyPushedking(position const& p)
        :
                color_piece_
                {
                        {p.num_pieces(black_c, pawn_c), p.num_pieces(black_c, king_c)},
                        {p.num_pieces(white_c, pawn_c), p.num_pieces(white_c, king_c)}
                }
        {}

        template<class Action>
        void make(Action const& m)
        {
                make_advance(m);
                make_promote(m);
                make_capture(m);
        }

        constexpr auto const& index(color c) const noexcept
        {
                return index_[xstd::to_underlying_type(c)];
        }

        constexpr auto const& count(color c) const noexcept
        {
                return count_[xstd::to_underlying_type(c)];
        }

        constexpr auto is_tracked(color c) const noexcept
        {
                return 0 < num_pieces(c, pawn_c) && 0 < num_pieces(c, king_c);
        }

        constexpr auto is_counted(color c) const noexcept
        {
                return 0 < count(c);
        }

        constexpr auto is_limited(color c) const noexcept
        {
                return count(c) == M;
        }

        template<class TabulationHash>
        friend auto hash_xor_accumulate(TabulationHash const& h, MostRecentlyPushedking const& mrp_king)
        {
                return
                        h.index(black_c)[mrp_king.index(black_c)] ^
                        h.index(white_c)[mrp_king.index(white_c)] ^
                        h.count(black_c)[mrp_king.count(black_c)] ^
                        h.count(white_c)[mrp_king.count(white_c)]
                ;
        }

private:
        template<class Action>
        void make_advance(Action const& m)
        {
                if (!is_tracked(m.to_move())) {
                        return;
                }

                if (!m.is_reversible()) {
                        return reset(m.to_move());
                }

                if (m.from() != index(m.to_move())) {
                        return init(m.to_move(), m.dest());
                }

                increment(m.to_move(), m.dest());
        }

        template<class Action>
        void make_promote(Action const& m)
        {
                if (!m.is_promotion()) {
                        return;
                }

                --num_pieces(m.to_move(), pawn_c);
                ++num_pieces(m.to_move(), king_c);
        }

        template<class Action>
        void make_capture(Action const& m)
        {
                if (!m.is_jump()) {
                        return;
                }

                if (
                        is_tracked(!m.to_move()) && (
                                num_pieces(!m.to_move(), pawn_c) == m.num_captured(pawn_c) ||
                                num_pieces(!m.to_move(), king_c) == m.num_captured(king_c) ||
                                (0 < m.num_captured(king_c) && m.captured(king_c).contains(index(!m.to_move())))
                        )
                ) {
                        reset(!m.to_move());
                }

                num_pieces(!m.to_move(), pawn_c) -= m.num_captured(pawn_c);
                num_pieces(!m.to_move(), king_c) -= m.num_captured(king_c);
        }

        constexpr void reset(color c)
        {
                assert(is_tracked(c));
                index(c) = N;
                count(c) = 0;
        }

        constexpr void init(color c, std::size_t dest_sq)
        {
                assert(is_tracked(c));
                assert(is_onboard(dest_sq));
                index(c) = dest_sq;
                count(c) = 1;
                assert(is_counted(c));
        }

        constexpr void increment(color c, std::size_t dest_sq)
        {
                assert(is_counted(c));
                assert(is_onboard(dest_sq));
                index(c) = dest_sq;
                ++count(c);
        }

        constexpr auto& index(color c) noexcept
        {
                return index_[xstd::to_underlying_type(c)];
        }

        constexpr auto& count(color c) noexcept
        {
                return count_[xstd::to_underlying_type(c)];
        }

        constexpr auto& num_pieces(color c, piece p) noexcept
        {
                return color_piece_[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        constexpr auto const& num_pieces(color c, piece p) const noexcept
        {
                return color_piece_[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        constexpr auto is_onboard(std::size_t sq) const noexcept
        {
                return sq < N;
        }
};

}       // namespace dctl::core
