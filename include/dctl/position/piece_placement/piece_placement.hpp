#pragma once
#include <dctl/board/mask.hpp>
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/position/piece_placement/zobrist.hpp>
#include <dctl/set_type.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <cassert>                      // assert

namespace dctl {

template<class Rules, class Board>
class PiecePlacement
{
        using Set = set_type<Board>;

        Set by_color[2];
        Set by_piece[2];

        template<Color ToMove>
        static constexpr auto promotion()
        {
                return board::Promotion<Board>::mask(ToMove);
        }

        bool invariant() const
        {
                auto constexpr squares = board::Squares<Board>::mask();

                return
                         disjoint(pieces(Color::black), pieces(Color::white)) &&
                         disjoint(pieces(Piece::pawn ), pieces(Piece::king )) &&
                         pieces() == (pieces(Piece::pawn) | pieces(Piece::king)) &&
                         disjoint(pieces(Color::black, Piece::pawn), promotion<Color::black>()) &&
                         disjoint(pieces(Color::white, Piece::pawn), promotion<Color::white>()) &&
                         pieces().is_subset_of(squares)
                ;
        }

public:
        PiecePlacement() = default;

        PiecePlacement(Set const& black, Set const& white, Set const& pawns, Set const& kings)
        :
                by_color{black, white},
                by_piece{pawns, kings}
        {
                assert(invariant());
        }

        template<class Move, class Index>
        void make(Move const& m, Index& hash)
        {
                using Zobrist = zobrist::PiecePlacement<set_type<Board>::size()>;

                pieces(m.to_move()).reset(m.from());
                pieces(m.to_move()).set  (m.dest());
                hash ^= Zobrist::pieces(m.to_move())[m.from()];
                hash ^= Zobrist::pieces(m.to_move())[m.dest()];

                if (m.is_jump()) {
                        pieces(!m.to_move()) ^= m.captured();
                        pieces(Piece::pawn ) ^= m.captured(Piece::pawn);
                        pieces(Piece::king ) ^= m.captured(Piece::king);
                        hash ^= zobrist::hash_xor_accumulate(m.captured()           , Zobrist::pieces(!m.to_move()));
                        hash ^= zobrist::hash_xor_accumulate(m.captured(Piece::pawn), Zobrist::pieces(Piece::pawn));
                        hash ^= zobrist::hash_xor_accumulate(m.captured(Piece::king), Zobrist::pieces(Piece::king));
                }

                if (m.is_promotion()) {
                        pieces(Piece::pawn).reset(m.from());
                        pieces(Piece::king).set  (m.dest());
                        hash ^= Zobrist::pieces(Piece::pawn)[m.from()];
                        hash ^= Zobrist::pieces(Piece::king)[m.dest()];
                } else {
                        pieces(m.with()).reset(m.from());
                        pieces(m.with()).set  (m.dest());
                        hash ^= Zobrist::pieces(m.with())[m.from()];
                        hash ^= Zobrist::pieces(m.with())[m.dest()];
                }
                assert(invariant());
        }

        auto pieces(Color c) const noexcept
        {
                return by_color[static_cast<std::size_t>(c)];
        }

        auto pieces(Piece p) const noexcept
        {
                return by_piece[static_cast<std::size_t>(p)];
        }

        auto pieces(Color c, Piece p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        auto pieces() const noexcept
        {
                return pieces(Color::black) | pieces(Color::white);
        }

        auto not_occupied() const noexcept
        {
                auto constexpr squares = board::Squares<Board>::mask();
                return squares ^ pieces();
        }

private:
        auto& pieces(Color c) noexcept
        {
                return by_color[static_cast<std::size_t>(c)];
        }

        auto& pieces(Piece p) noexcept
        {
                return by_piece[static_cast<std::size_t>(p)];
        }
};

template<class TabulationHash, class Rules, class Board>
auto hash_xor_accumulate(TabulationHash const& h, PiecePlacement<Rules, Board> const& p)
{
        return
                zobrist::hash_xor_accumulate(p.pieces(Color::black), h.pieces(Color::black)) ^
                zobrist::hash_xor_accumulate(p.pieces(Color::white), h.pieces(Color::white)) ^
                zobrist::hash_xor_accumulate(p.pieces(Piece::pawn ), h.pieces(Piece::pawn )) ^
                zobrist::hash_xor_accumulate(p.pieces(Piece::king ), h.pieces(Piece::king ))
        ;
}

}       // namespace dctl
