#pragma once
#include <cassert>                      // assert
#include <tuple>
#include <type_traits>                  // false_type, true_type
#include <dctl/bit.hpp>
#include <dctl/board/mask.hpp>
#include <dctl/move/move.hpp>
#include <dctl/position/mru_king/mru_king.hpp>
#include <dctl/position/mru_king/zobrist.hpp>
#include <dctl/position/side.hpp>
#include <dctl/position/position_fwd.hpp>
#include <dctl/position/predicates_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/zobrist/accumulate.hpp>
#include <dctl/zobrist/material.hpp>

namespace dctl {

template<class Rules, class Board>
uint64_t zobrist_hash(Position<Rules, Board> const& p);

template<class Rules, class Board>
struct BasePosition
{
public:
        using rules_type = Rules;
        using board_type = Board;
        using Set = typename Board::set_type;
        using TreeIterator = Position const*;
        static const auto gap = rules::initial_gap<Rules>::value + Board::height % 2;

        // initialize with a set of bitboards and a color
        BasePosition(Set black_pieces, Set white_pieces, Set kings, bool to_move)
        :
                to_move_{to_move}
        {
                pieces_[Side::black] = black_pieces;
                pieces_[Side::white] = white_pieces;
                kings_ = kings;
                assert(material_invariant());
                hash_ = zobrist_hash(*this);
        }

        static BasePosition initial(int separation = gap)
        {
                return Position{
                        board::Initial<Board>::mask(Side::black, separation),
                        board::Initial<Board>::mask(Side::white, separation),
                        Set{},
                        Side::white
                };
        }

        // queries

        auto hash() const
        {
                return hash_;
        }

        auto const& material() const
        {
                return std::forward_as_tuple(
                        pieces(Side::black),
                        pieces(Side::white),
                        kings()
                );
        }

        // black or white pawns
        auto pawns(bool color) const
        {
                return pieces(color) & ~kings();
        }

        // black or white kings
        auto kings(bool color) const
        {
                return pieces(color) & kings();
        }

        // black or white pieces
        auto pieces(bool color) const
        {
                return pieces_[color];
        }

        // black and white pawns
        auto pawns() const
        {
                return pieces() & ~kings();
        }

        // black and white kings
        auto kings() const
        {
                return kings_;
        }

        // black and white pieces
        auto pieces() const
        {
                return pieces(Side::black) ^ pieces(Side::white);
        }

        // side to move
        auto to_move() const
        {
                return to_move_;
        }

        template<class Move>
        void make_incremental(Move const& m)
        {
                using Zobrist = random::Incremental<Board::set_type::N>;

                pieces_[to_move_].reset(m.from());
                pieces_[to_move_].set(m.dest());
                hash_ ^= Zobrist::pieces[to_move_][static_cast<std::size_t>(m.from())];
                hash_ ^= Zobrist::pieces[to_move_][static_cast<std::size_t>(m.dest())];
                if (m.is_with_king()) {
                        kings_.reset(m.from());
                        kings_.set(m.dest());
                        hash_ ^= Zobrist::kings[static_cast<std::size_t>(m.from())];
                        hash_ ^= Zobrist::kings[static_cast<std::size_t>(m.dest())];
                } else if (m.is_promotion()) {
                        kings_.set(m.dest());
                        hash_ ^= Zobrist::kings[static_cast<std::size_t>(m.dest())];
                }
                if (m.is_jump()) {
                        pieces_[!to_move_] ^= m.captured_pieces();
                        kings_ ^= m.captured_kings();
                        hash_ ^= zobrist::accumulate(m.captured_pieces(), Zobrist::pieces[!to_move_]);
                        hash_ ^= zobrist::accumulate(m.captured_kings() , Zobrist::kings           );
                }
                to_move_.flip();
                hash_ ^= Zobrist::to_move[Side::pass];
        }

        // post-conditions for the constructors and modifiers
        bool material_invariant() const
        {
                auto constexpr squares = board::Squares<Board>::mask();
                return
                        bit::set_includes(squares, pieces()) &&
                        bit::set_includes(pieces(), kings()) &&
                        !bit::set_intersects(pieces(Side::black), pieces(Side::white))
                ;
        }

        bool hash_invariant() const
        {
                return hash_ == zobrist_hash(*this);
        }

        // representation

        Set pieces_[2];
        Set kings_;
        Side to_move_{};
};

template<class Position>
auto grand_parent(Position const& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

template<class Position>
auto active_color(Position const& p)
{
        return p.to_move();
}

template<class Position>
auto passive_color(Position const& p)
{
        return !p.to_move();
}

template<class Position>
decltype(auto) active_restricted(Position const& p)
{
        return p.restricted()[active_color(p)];
}

template<class Position>
decltype(auto) passive_restricted(Position const& p)
{
        return p.restricted()[passive_color(p)];
}

template<class Rules, class Board>
uint64_t zobrist_hash(BasePosition<Rules, Board> const& p)
{
        using Zobrist = random::Incremental<Board::set_type::N>;

        return
                zobrist::accumulate(p.pieces(Side::black), Zobrist::pieces[Side::black]) ^
                zobrist::accumulate(p.pieces(Side::white), Zobrist::pieces[Side::white]) ^
                zobrist::accumulate(p.kings(),             Zobrist::kings              ) ^
                Zobrist::to_move[p.to_move()]
        ;
}

}       // namespace dctl
