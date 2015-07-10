#pragma once
#include <dctl/board/mask.hpp>
#include <dctl/player.hpp>
#include <dctl/piece.hpp>
#include <dctl/set_type.hpp>
#include <dctl/type_traits.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <cassert>                      // assert

namespace dctl {

template<class Rules, class Board>
class PiecePlacement
{
        using set_type = get_set_type<Board>;

        set_type by_color[2];
        set_type by_piece[2];

        auto invariant() const
        {
                auto constexpr squares = board::Squares<Board>::mask();

                return
                         disjoint(pieces(Player::black), pieces(Player::white)) &&
                         disjoint(pieces(Piece::pawn ), pieces(Piece::king )) &&
                         (pieces(Player::black) | pieces(Player::white)) == (pieces(Piece::pawn) | pieces(Piece::king)) &&
                         disjoint(pieces(Player::black, Piece::pawn), board::Promotion<Board>::mask(Player::black)) &&
                         disjoint(pieces(Player::white, Piece::pawn), board::Promotion<Board>::mask(Player::white)) &&
                         pieces().is_subset_of(squares)
                ;
        }

public:
        PiecePlacement() = default;

        PiecePlacement(set_type const& black, set_type const& white, set_type const& pawns, set_type const& kings)
        :
                by_color{black, white},
                by_piece{pawns, kings}
        {
                assert(invariant());
        }

        template<class Action>
        auto make(Action const& a)
        {
                pieces(a.to_move()).reset(a.from());
                pieces(a.to_move()).set  (a.dest());
                pieces(a.with()).reset(a.from());
                pieces(a.into()).set  (a.dest());

                if (a.is_jump()) {
                        pieces(!a.to_move()) ^= a.captured();
                        pieces(Piece::pawn ) ^= a.captured(Piece::pawn);
                        pieces(Piece::king ) ^= a.captured(Piece::king);
                }

                assert(invariant());
        }

        auto const& pieces(Player c) const noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto const& pieces(Piece p) const noexcept
        {
                return by_piece[xstd::to_underlying_type(p)];
        }

        auto pieces(Player c, Piece p) const noexcept
        {
                return pieces(c) & pieces(p);
        }

        auto pieces() const noexcept
        {
                return pieces(Player::black) | pieces(Player::white);
        }

        auto not_occupied() const noexcept
        {
                auto constexpr squares = board::Squares<Board>::mask();
                return squares ^ pieces();
        }

        auto num_pieces(Player c, Piece p) const noexcept
        {
                return pieces(c, p).count();
        }

        template<class TabulationHash>
        friend auto hash_xor_accumulate(TabulationHash const& h, PiecePlacement const& p)
        {
                return
                        zobrist::hash_xor_accumulate(h.pieces(Player::black), p.pieces(Player::black)) ^
                        zobrist::hash_xor_accumulate(h.pieces(Player::white), p.pieces(Player::white)) ^
                        zobrist::hash_xor_accumulate(h.pieces(Piece::pawn ), p.pieces(Piece::pawn )) ^
                        zobrist::hash_xor_accumulate(h.pieces(Piece::king ), p.pieces(Piece::king ))
                ;
        }

        template<class TabulationHash, class Action>
        friend auto hash_xor_accumulate(TabulationHash const& h, Action const& m, xstd::type_is<PiecePlacement>)
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
        auto& pieces(Player c) noexcept
        {
                return by_color[xstd::to_underlying_type(c)];
        }

        auto& pieces(Piece p) noexcept
        {
                return by_piece[xstd::to_underlying_type(p)];
        }
};

}       // namespace dctl
