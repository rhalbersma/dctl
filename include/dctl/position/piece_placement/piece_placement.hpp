#pragma once
#include <dctl/board/mask.hpp>
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/set_type.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <xstd/type_traits.hpp>         // to_underlying_type
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

        auto invariant() const
        {
                auto constexpr squares = board::Squares<Board>::mask();

                return
                         disjoint(pieces(Color::black), pieces(Color::white)) &&
                         disjoint(pieces(Piece::pawn ), pieces(Piece::king )) &&
                         (pieces(Color::black) | pieces(Color::white)) == (pieces(Piece::pawn) | pieces(Piece::king)) &&
                         disjoint(pieces(Color::black, Piece::pawn), board::Promotion<Board>::mask(Color::black)) &&
                         disjoint(pieces(Color::white, Piece::pawn), board::Promotion<Board>::mask(Color::white)) &&
                         pieces().is_subset_of(squares)
                ;
        }

public:
        PiecePlacement(Set const& black, Set const& white, Set const& pawns, Set const& kings)
        :
                by_color{black, white},
                by_piece{pawns, kings}
        {
                assert(invariant());
        }

        PiecePlacement() = default;

        template<class Move>
        auto make(Move const& m)
        {
                pieces(m.to_move()).reset(m.from());
                pieces(m.to_move()).set  (m.dest());
                pieces(m.with()).reset(m.from());
                pieces(m.into()).set  (m.dest());

                if (m.is_jump()) {
                        pieces(!m.to_move()) ^= m.captured();
                        pieces(Piece::pawn ) ^= m.captured(Piece::pawn);
                        pieces(Piece::king ) ^= m.captured(Piece::king);
                }

                assert(invariant());
        }

        auto const& pieces(Color c) const noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto const& pieces(Piece p) const noexcept
        {
                return by_piece[xstd::to_underlying_type(p)];
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

        auto num_pieces(Color c, Piece p) const noexcept
        {
                return pieces(c, p).count();
        }

        template<class TabulationHash>
        friend auto hash_xor_accumulate(TabulationHash const& h, PiecePlacement const& p)
        {
                return
                        zobrist::hash_xor_accumulate(h.pieces(Color::black), p.pieces(Color::black)) ^
                        zobrist::hash_xor_accumulate(h.pieces(Color::white), p.pieces(Color::white)) ^
                        zobrist::hash_xor_accumulate(h.pieces(Piece::pawn ), p.pieces(Piece::pawn )) ^
                        zobrist::hash_xor_accumulate(h.pieces(Piece::king ), p.pieces(Piece::king ))
                ;
        }

        template<class TabulationHash, class Move>
        friend auto hash_xor_accumulate(TabulationHash const& h, Move const& m, xstd::type_is<PiecePlacement>)
        {
                std::size_t hash = 0;

                hash ^= h.pieces(m.to_move())[m.from()];
                hash ^= h.pieces(m.to_move())[m.dest()];
                hash ^= h.pieces(m.with())[m.from()];
                hash ^= h.pieces(m.into())[m.dest()];

                if (m.is_jump()) {
                        hash ^= zobrist::hash_xor_accumulate(h.pieces(!m.to_move()), m.captured());
                        hash ^= zobrist::hash_xor_accumulate(h.pieces(Piece::pawn ), m.captured(Piece::pawn));
                        hash ^= zobrist::hash_xor_accumulate(h.pieces(Piece::king ), m.captured(Piece::king));
                }

                return hash;
        }

private:
        auto& pieces(Color c) noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto& pieces(Piece p) noexcept
        {
                return by_piece[xstd::to_underlying_type(p)];
        }
};

}       // namespace dctl
