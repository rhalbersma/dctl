#pragma once
#include <dctl/board/mask.hpp>
#include <dctl/position/color.hpp>
#include <dctl/position/piece_placement/zobrist.hpp>
#include <dctl/set_type.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <cassert>                      // assert

namespace dctl {

template<class Rules, class Board>
class PiecePlacement
{
public:
        using Set = set_type<Board>;

        PiecePlacement() = default;

        PiecePlacement(Set black_pieces, Set white_pieces, Set kings)
        :
                pieces_{black_pieces, white_pieces},
                kings_{kings}
        {
                assert(invariant());
        }

        PiecePlacement(Set black_pawns, Set black_kings, Set white_pawns, Set white_kings)
        :
                pieces_{black_pawns | black_kings, white_pawns | white_kings},
                kings_{black_kings | white_kings}
        {
                assert(invariant());
        }

        // modifiers

        template<class Move, class Index>
        void make(Move const& m, Index& hash)
        {
                using Zobrist = random::PiecePlacement<set_type<Board>::size()>;
                pieces_[m.active_color()].reset(m.from());
                pieces_[m.active_color()].set(m.dest());
                hash ^= Zobrist::pieces[m.active_color()][m.from()];
                hash ^= Zobrist::pieces[m.active_color()][m.dest()];
                if (m.is_with_king()) {
                        kings_.reset(m.from());
                        kings_.set(m.dest());
                        hash ^= Zobrist::kings[m.from()];
                        hash ^= Zobrist::kings[m.dest()];
                } else if (m.is_promotion()) {
                        kings_.set(m.dest());
                        hash ^= Zobrist::kings[m.dest()];
                }
                if (m.is_jump()) {
                        pieces_[!m.active_color()] ^= m.captured_pieces();
                        kings_ ^= m.captured_kings();
                        hash ^= zobrist::accumulate(m.captured_pieces(), Zobrist::pieces[!m.active_color()]);
                        hash ^= zobrist::accumulate(m.captured_kings() , Zobrist::kings                    );
                }
                assert(invariant());
        }

        // observers

        auto kings(bool color) const
        {
                return pieces_[color] & kings_;
        }

        auto pawns(bool color) const
        {
                return pieces_[color] - kings_;
        }

        auto pieces(bool color) const
        {
                return pieces_[color];
        }

        auto kings() const
        {
                return kings_;
        }

        auto pawns() const
        {
                return pieces() - kings_;
        }

        auto pieces() const
        {
                return pieces_[Color::black] | pieces_[Color::white];
        }

        auto not_occupied() const
        {
                auto constexpr squares = board::Squares<Board>::mask();
                return squares ^ pieces();
        }

private:
        // contracts

        bool invariant() const
        {
                auto constexpr squares = board::Squares<Board>::mask();
                return
                         kings().is_subset_of(pieces()) &&
                         pieces().is_subset_of(squares) &&
                         disjoint(pieces(Color::black), pieces(Color::white))
                ;
        }

        // representation

        Set pieces_[2];
        Set kings_;
};

template<class Rules, class Board>
auto init_hash(PiecePlacement<Rules, Board> const& m)
{
        using Zobrist = random::PiecePlacement<set_type<Board>::size()>;
        return
                zobrist::accumulate(m.pieces(Color::black), Zobrist::pieces[Color::black]) ^
                zobrist::accumulate(m.pieces(Color::white), Zobrist::pieces[Color::white]) ^
                zobrist::accumulate(m.kings()             , Zobrist::kings               )
        ;
}

}       // namespace dctl
