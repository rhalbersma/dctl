#pragma once
#include <dctl/color_piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cstddef>

namespace dctl {

template<class Rules, class Board>
class MostRecentlyPushedKings
{
public:
        enum { M = max_same_king_push_v<Rules> };
        enum { N = set_t<Board>::size() };

private:
        std::size_t index_[2]{ N, N };
        std::size_t count_[2]{};
        std::size_t color_piece_[2][2];

public:
        template<class BaseState>
        constexpr MostRecentlyPushedKings(BaseState const& p)
        :
                color_piece_
                {
                        {p.num_pieces(Color::black, Piece::pawn), p.num_pieces(Color::black, Piece::king)},
                        {p.num_pieces(Color::white, Piece::pawn), p.num_pieces(Color::white, Piece::king)}
                }
        {}

        template<class Action>
        void make(Action const& m)
        {
                make_advance(m);
                make_promote(m);
                make_capture(m);
        }

        constexpr auto const& index(Color c) const noexcept
        {
                return index_[xstd::to_underlying_type(c)];
        }

        constexpr auto const& count(Color c) const noexcept
        {
                return count_[xstd::to_underlying_type(c)];
        }

        constexpr auto is_tracked(Color c) const noexcept
        {
                return 0 < num_pieces(c, Piece::pawn) && 0 < num_pieces(c, Piece::king);
        }

        constexpr auto is_counted(Color c) const noexcept
        {
                return 0 < count(c);
        }

        constexpr auto is_limited(Color c) const noexcept
        {
                return count(c) == M;
        }

        template<class TabulationHash>
        friend auto hash_xor_accumulate(TabulationHash const& h, MostRecentlyPushedKings const& mrp_kings)
        {
                return
                        h.index(Color::black)[mrp_kings.index(Color::black)] ^
                        h.index(Color::white)[mrp_kings.index(Color::white)] ^
                        h.count(Color::black)[mrp_kings.count(Color::black)] ^
                        h.count(Color::white)[mrp_kings.count(Color::white)]
                ;
        }

private:
        template<class Action>
        void make_advance(Action const& m)
        {
                if (!is_tracked(m.to_move()))
                        return;

                if (!m.is_reversible())
                        return reset(m.to_move());

                if (m.from() != index(m.to_move()))
                        return init(m.to_move(), m.dest());

                increment(m.to_move(), m.dest());
        }

        template<class Action>
        void make_promote(Action const& m)
        {
                if (!m.is_promotion())
                        return;

                --num_pieces(m.to_move(), Piece::pawn);
                ++num_pieces(m.to_move(), Piece::king);
        }

        template<class Action>
        void make_capture(Action const& m)
        {
                if (!m.is_jump())
                        return;

                if (
                        is_tracked(!m.to_move()) && (
                                num_pieces(!m.to_move(), Piece::pawn) == m.num_captured(Piece::pawn) ||
                                num_pieces(!m.to_move(), Piece::king) == m.num_captured(Piece::king) ||
                                (0 < m.num_captured(Piece::king) && m.captured(Piece::king).test(index(!m.to_move())))
                        )
                )
                        reset(!m.to_move());

                num_pieces(!m.to_move(), Piece::pawn) -= m.num_captured(Piece::pawn);
                num_pieces(!m.to_move(), Piece::king) -= m.num_captured(Piece::king);
        }

        constexpr void reset(Color c)
        {
                assert(is_tracked(c));
                index(c) = N;
                count(c) = 0;
        }

        constexpr void init(Color c, std::size_t dest_sq)
        {
                assert(is_tracked(c));
                assert(is_onboard(dest_sq));
                index(c) = dest_sq;
                count(c) = 1;
                assert(is_counted(c));
        }

        constexpr void increment(Color c, std::size_t dest_sq)
        {
                assert(is_counted(c));
                assert(is_onboard(dest_sq));
                index(c) = dest_sq;
                ++count(c);
        }

        constexpr auto& index(Color c) noexcept
        {
                return index_[xstd::to_underlying_type(c)];
        }

        constexpr auto& count(Color c) noexcept
        {
                return count_[xstd::to_underlying_type(c)];
        }

        constexpr auto& num_pieces(Color c, Piece p) noexcept
        {
                return color_piece_[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        constexpr auto const& num_pieces(Color c, Piece p) const noexcept
        {
                return color_piece_[xstd::to_underlying_type(c)][xstd::to_underlying_type(p)];
        }

        constexpr auto is_onboard(std::size_t sq) const noexcept
        {
                return sq < N;
        }
};

}       // namespace dctl
