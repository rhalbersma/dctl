#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/rule_traits.hpp>
#include <dctl/set_type.hpp>
#include <cstddef>

namespace dctl {

template<class Rules, class Board>
class MostRecentlyPushedKings
{
        std::size_t index_[2]{};
        std::size_t moves_[2]{};
        std::size_t by_color_piece_[2][2];

public:
        enum { M = max_same_king_push_v<Rules> };
        enum { N = set_type<Board>::size() };

        template<class PiecePlacement>
        constexpr MostRecentlyPushedKings(PiecePlacement const& p)
        :
                by_color_piece_{
                        {p.num_pieces(Color::black, Piece::pawn), p.num_pieces(Color::black, Piece::king)},
                        {p.num_pieces(Color::white, Piece::pawn), p.num_pieces(Color::white, Piece::king)}
                }
        {}

        template<class Move>
        void make(Move const& m)
        {
                make_increment(m);
                make_promotion(m);
                make_jump(m);
        }

        constexpr auto square(Color c) const noexcept
        {
                return index(c) - 1;
        }

        constexpr auto is_tracked(Color c) const noexcept
        {
                return 0 < num_pieces(c, Piece::pawn) && 0 < num_pieces(c, Piece::king);
        }

        constexpr auto is_restricted(Color c) const noexcept
        {
                return moves(c) == M;
        }

        template<class TabulationHash>
        friend auto hash_xor_accumulate(TabulationHash const& h, MostRecentlyPushedKings const& mrp_kings)
        {
                return
                        h.index(Color::black)[mrp_kings.index(Color::black)] ^
                        h.moves(Color::black)[mrp_kings.moves(Color::black)] ^
                        h.index(Color::white)[mrp_kings.index(Color::white)] ^
                        h.moves(Color::white)[mrp_kings.moves(Color::white)]
                ;
        }

private:
        template<class Move>
        void make_increment(Move const& m)
        {
                if (!is_tracked(m.to_move()))
                        return;

                if (!m.is_reversible())
                        return reset(m.to_move());

                if (m.from() != square(m.to_move()))
                        return init(m.to_move(), m.dest());

                increment(m.to_move(), m.dest());
        }

        template<class Move>
        void make_promotion(Move const& m)
        {
                if (!m.is_promotion())
                        return;

                --num_pieces(m.to_move(), Piece::pawn);
                ++num_pieces(m.to_move(), Piece::king);
        }

        template<class Move>
        void make_jump(Move const& m)
        {
                if (!m.is_jump())
                        return;

                if (
                        is_tracked(!m.to_move()) && (
                                num_pieces(!m.to_move(), Piece::pawn) == m.num_captured(Piece::pawn) ||
                                num_pieces(!m.to_move(), Piece::king) == m.num_captured(Piece::king) ||
                                (0 < m.num_captured(Piece::king) && m.captured(Piece::king).test(square(!m.to_move())))
                        )
                )
                        reset(!m.to_move());

                num_pieces(!m.to_move(), Piece::pawn) -= m.num_captured(Piece::pawn);
                num_pieces(!m.to_move(), Piece::king) -= m.num_captured(Piece::king);
        }

        constexpr void reset(Color c)
        {
                assert(is_tracked(c));
                index(c) = 0;
                moves(c) = 0;
        }

        constexpr void init(Color c, std::size_t dest_sq)
        {
                assert(is_tracked(c));
                assert(dest_sq < N);
                index(c) = dest_sq + 1;
                moves(c) = 1;
                assert(1 <= moves(c) && moves(c) <= M);
        }

        constexpr void increment(Color c, std::size_t dest_sq)
        {
                assert(is_tracked(c));
                assert(dest_sq < N);
                assert(1 <= moves(c) && moves(c) < M);
                index(c) = dest_sq + 1;
                ++moves(c);
                assert(2 <= moves(c) && moves(c) <= M);
        }

        constexpr auto& index(Color c) noexcept
        {
                return index_[static_cast<std::size_t>(c)];
        }

        constexpr auto const& index(Color c) const noexcept
        {
                return index_[static_cast<std::size_t>(c)];
        }

        constexpr auto& moves(Color c) noexcept
        {
                return moves_[static_cast<std::size_t>(c)];
        }

        constexpr auto const& moves(Color c) const noexcept
        {
                return moves_[static_cast<std::size_t>(c)];
        }

        constexpr auto& num_pieces(Color c, Piece p) noexcept
        {
                return by_color_piece_[static_cast<std::size_t>(c)][static_cast<std::size_t>(p)];
        }

        constexpr auto const& num_pieces(Color c, Piece p) const noexcept
        {
                return by_color_piece_[static_cast<std::size_t>(c)][static_cast<std::size_t>(p)];
        }
};

}       // namespace dctl
