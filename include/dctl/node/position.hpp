#pragma once
#include <cassert>                      // assert
#include <type_traits>                  // false_type, true_type
#include <tuple>
#include <dctl/zobrist/hash.hpp>
#include <dctl/node/position_fwd.hpp>
#include <dctl/move/move.hpp>
#include <dctl/node/restricted.hpp>
#include <dctl/node/side.hpp>
#include <dctl/node/predicates_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/ply.hpp>         // PlyCount
#include <dctl/utility/int.hpp>         // HashIndex

#include <dctl/bit/algorithm.hpp>
#include <dctl/bit/predicates.hpp>
#include <dctl/board/mask/initial.hpp>
#include <dctl/board/mask/squares.hpp>

namespace dctl {

template
<
        class Rules,
        class Board
>
struct Position
{
public:
        using rules_type = Rules;
        using board_type = Board;
        using Set = typename Board::set_type;
        using TreeIterator = Position const*;
        static const auto gap = rules::initial_gap<Rules>::value + Board::height % 2;

        // initialize with a set of bitboards and a color
        Position(Set black_pieces, Set white_pieces, Set kings, bool to_move)
        :
                to_move_{to_move}
        {
                pieces_[Side::black] = black_pieces;
                pieces_[Side::white] = white_pieces;
                kings_ = kings;
                assert(material_invariant());
                hash_ = zobrist::hash(*this);
        }

        static Position initial(int separation = gap)
        {
                return Position{
                        board::Initial<Board>::mask(Side::black, separation),
                        board::Initial<Board>::mask(Side::white, separation),
                        Set{},
                        Side::white
                };
        }

        // queries
        TreeIterator parent() const
        {
                return parent_;
        }

        HashIndex hash() const
        {
                return hash_;
        }

        auto const& material() const
        {
                return std::tie(pieces(Side::black), pieces(Side::white), kings());
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

        auto const& restricted() const
        {
                return restricted_;
        }

        auto const& restricted(bool color) const
        {
                return restricted_[color];
        }

        auto reversible_moves() const
        {
                return reversible_moves_;
        }

        auto distance_to_root() const
        {
                return distance_to_root_;
        }

        // side to move
        auto to_move() const
        {
                return to_move_;
        }

        template<class Move>
        void make(Move const& m)
        {
                //assert(is_pseudo_legal(*this, m));

                make_irreversible(m);
                make_incremental(m);

                assert(material_invariant());
                assert(hash_index_invariant());
        }
        
        void attach(Position const& other)
        {
                parent_ = &other;       // link the pointers
        }

private:
        // implementation

        template<class Move>
        void make_irreversible(Move const& m)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                make_irreversible(m, rules::is_restricted_same_king_moves<Rules>{});
        }

        // overload for restricted consecutive moves with the same king
        template<class Move>
        void make_irreversible(Move const& m, std::true_type)
        {
                make_irreversible(m, std::false_type());
                make_restricted(m);
        }

        // overload for unrestricted consecutive moves with the same king
        template<class Move>
        void make_irreversible(Move const& m, std::false_type)
        {
                make_reversible_moves(m);
                make_distance_to_root();
        }

        template<class Move>
        void make_reversible_moves(Move const& m)
        {
                if (m.is_reversible())
                        ++reversible_moves_;
                else
                        reversible_moves_ = 0;
        }

        void make_distance_to_root()
        {
                ++distance_to_root_;
        }

        template<class Move>
        void make_restricted(Move const& m)
        {
                make_active_king_moves(m);
                make_passive_king_moves(m);
        }

        template<class Move>
        void make_active_king_moves(Move const& m)
        {
                KingMoves& restricted = restricted_[active_color(*this)];

                if (!active_kings(*this).empty() && !active_pawns(*this).empty()) {
                        if (restricted.moves())
                                hash_ ^= zobrist::hash(std::make_pair(restricted, active_color(*this)));

                        if (!m.is_reversible()) {
                                if (restricted.moves())
                                        restricted.reset();
                                return;
                        }

                        if (restricted.moves() && (restricted.king() == m.from())) {
                                // a consecutive irreversible move with the same king
                                assert(!is_max<Rules>(restricted.moves()));
                                restricted.increment(m.dest());
                        } else {
                                // a first irreversible move with a new king
                                assert(!restricted.moves() || active_kings(*this).size() > 0);
                                restricted.init(m.dest());
                        }
                        hash_ ^= zobrist::hash(std::make_pair(restricted, active_color(*this)));
                }
        }

        template<class Move>
        void make_passive_king_moves(Move const& m)
        {
                KingMoves& restricted = restricted_[passive_color(*this)];

                if (
                        restricted.moves() && m.is_jump() &&
                        (
                                m.captured_pieces().test(restricted.king()) ||
                                bit::set_includes(m.captured_pieces(), passive_pawns(*this))
                        )
                ) {
                        hash_ ^= zobrist::hash(std::make_pair(restricted, passive_color(*this)));
                        restricted.reset();
                }
        }

        template<class Move>
        void make_incremental(Move const& m)
        {
                make_material(m);
                make_to_move();
        }

        template<class Move>
        void make_material(Move const& m)
        {
                pieces_[to_move_].reset(m.from());
                pieces_[to_move_].set(m.dest());
                if (m.is_with_king()) {
                        kings_.reset(m.from());
                        kings_.set(m.dest());
                } else if (m.is_promotion()) {
                        kings_.set(m.dest());
                }
                if (m.is_jump()) {
                        pieces_[!to_move_] ^= m.captured_pieces();
                        kings_ ^= m.captured_kings();
                }

                hash_ ^= zobrist::hash(std::make_pair(m, active_color(*this)));
        }

        void make_to_move()
        {
                to_move_ ^= Side::pass;
                hash_ ^= zobrist::hash(bool(Side::pass));
        }

        // post-conditions for the constructors and modifiers
        bool material_invariant() const
        {
                auto constexpr squares = board::Squares<Board>::mask();
                return
                        bit::set_exclusive(pieces(Side::black), pieces(Side::white)) &&
                        bit::set_includes(pieces(), kings()) &&
                        bit::set_includes(squares, pieces())
                ;
        }

        bool hash_index_invariant() const
        {
                return hash_ == zobrist::hash(*this);
        }

        // representation
        Set pieces_[2];   // black and white pieces
        Set kings_;       // kings
        TreeIterator parent_{};
        HashIndex hash_{};
        Restricted restricted_{};
        int reversible_moves_{};
        int distance_to_root_{};
        bool to_move_{};
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

}       // namespace dctl
