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

        // modifiers

        template<class Move, class Index>
        void make(Move const& m, Index& hash)
        {
                using Zobrist = zobrist::PiecePlacement<set_type<Board>::size()>;

                by_color[static_cast<std::size_t>(m.to_move())].reset(m.from());
                by_color[static_cast<std::size_t>(m.to_move())].set  (m.dest());
                hash ^= Zobrist::by_color[static_cast<std::size_t>(m.to_move())][m.from()];
                hash ^= Zobrist::by_color[static_cast<std::size_t>(m.to_move())][m.dest()];

                if (m.is_promotion()) {
                        by_piece[static_cast<std::size_t>(Piece::pawn)].reset(m.from());
                        by_piece[static_cast<std::size_t>(Piece::king)].set  (m.dest());
                        hash ^= Zobrist::by_piece[static_cast<std::size_t>(Piece::pawn)][m.from()];
                        hash ^= Zobrist::by_piece[static_cast<std::size_t>(Piece::king)][m.dest()];
                } else {
                        by_piece[static_cast<std::size_t>(m.with())].reset(m.from());
                        by_piece[static_cast<std::size_t>(m.with())].set  (m.dest());
                        hash ^= Zobrist::by_piece[static_cast<std::size_t>(m.with())][m.from()];
                        hash ^= Zobrist::by_piece[static_cast<std::size_t>(m.with())][m.dest()];
                }
                if (m.is_jump()) {
                        by_color[static_cast<std::size_t>(!m.to_move())] ^= m.captured();
                        by_piece[static_cast<std::size_t>(Piece::pawn) ] ^= m.captured(Piece::pawn);
                        by_piece[static_cast<std::size_t>(Piece::king) ] ^= m.captured(Piece::king);
                        hash ^= zobrist::hash_xor_accumulate(Zobrist::by_color[static_cast<std::size_t>(!m.to_move())], m.captured());
                        hash ^= zobrist::hash_xor_accumulate(Zobrist::by_piece[static_cast<std::size_t>(Piece::pawn )], m.captured(Piece::pawn));
                        hash ^= zobrist::hash_xor_accumulate(Zobrist::by_piece[static_cast<std::size_t>(Piece::king )], m.captured(Piece::king));
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
};

template<class TabulationHash, class Rules, class Board>
auto hash_xor_accumulate(TabulationHash const& h, PiecePlacement<Rules, Board> const& p)
{
        return
                zobrist::hash_xor_accumulate(h.by_color[static_cast<std::size_t>(Color::black)], p.pieces(Color::black)) ^
                zobrist::hash_xor_accumulate(h.by_color[static_cast<std::size_t>(Color::white)], p.pieces(Color::white)) ^
                zobrist::hash_xor_accumulate(h.by_piece[static_cast<std::size_t>(Piece::pawn )], p.pieces(Piece::pawn )) ^
                zobrist::hash_xor_accumulate(h.by_piece[static_cast<std::size_t>(Piece::king )], p.pieces(Piece::king ))
        ;
}

}       // namespace dctl
