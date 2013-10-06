#pragma once
#include <cassert>                      // assert
#include <type_traits>                  // false_type, true_type
#include <dctl/bit/bit.hpp>
#include <dctl/zobrist/hash.hpp>
#include <dctl/node/position_fwd.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/restricted.hpp>
#include <dctl/node/side.hpp>
#include <dctl/node/predicates_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>         // PlyCount, HashIndex
#include <dctl/bit/algorithm.hpp>
#include <dctl/bit/bitboard.hpp>        // BitBoard

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
        using board_type = Board ;
        using TreeIterator = Position const*;
        static const int gap = rules::initial_gap<rules_type>::value + board_type::height % 2;

        /*
        Position()
        {
                // no-op
        }
        */
        // initialize with a set of bitboards and a color
        Position(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings, bool to_move)
        :
                parent_(nullptr),
                material_(black_pieces, white_pieces, kings),
                reversible_moves_(0),
                distance_to_root_(0),
                to_move_(to_move)
        {
                hash_index_ = zobrist::hash(*this);
                assert(material_invariant());
        }

        // initial position
        static Position initial()
        {
                return initial<gap>();
        }

        template<int N>
        static Position initial()
        {
                return Position(
                        Board::initial_mask[Side::black][N],
                        Board::initial_mask[Side::white][N],
                        0,
                        Side::white
                );
        }

        // queries
        TreeIterator parent() const
        {
                return parent_;
        }

        HashIndex hash_index() const
        {
                return hash_index_;
        }

        Material const& material() const
        {
                return material_;
        }

        Material const& key() const
        {
                return material();
        }

        Restricted const& restricted() const
        {
                return restricted_;
        }

        KingMoves const& restricted(bool color) const
        {
                return restricted_[color];
        }

        PlyCount reversible_moves() const
        {
                return reversible_moves_;
        }

        PlyCount distance_to_root() const
        {
                return distance_to_root_;
        }

        // side to move
        bool to_move() const
        {
                return to_move_;
        }

        void make(Move const& m)
        {
                assert(is_pseudo_legal(*this, m));

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

        void make_irreversible(Move const& m)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                make_irreversible(m, rules::is_restricted_same_king_moves<Rules>());
        }

        // overload for restricted consecutive moves with the same king
        void make_irreversible(Move const& m, std::true_type)
        {
                make_irreversible(m, std::false_type());
                make_restricted(m);
        }

        // overload for unrestricted consecutive moves with the same king
        void make_irreversible(Move const& m, std::false_type)
        {
                make_reversible_moves(m);
                make_distance_to_root();
        }

        void make_reversible_moves(Move const& m)
        {
                if (is_reversible(*this, m))
                        ++reversible_moves_;
                else
                        reversible_moves_ = 0;
        }

        void make_distance_to_root()
        {
                ++distance_to_root_;
        }

        void make_restricted(Move const& m)
        {
                make_active_king_moves(m);
                make_passive_king_moves(m);
        }

        void make_active_king_moves(Move const& m)
        {
                KingMoves& restricted = restricted_[active_color(*this)];

                if (active_kings(*this) && active_pawns(*this)) {
                        if (restricted.moves())
                                hash_index_ ^= zobrist::hash(std::make_pair(restricted, active_color(*this)));

                        if (!is_reversible(*this, m)) {
                                if (restricted.moves())
                                        restricted.reset();
                                return;
                        }

                        if (restricted.moves() && bit::is_element(restricted.king(), from_sq(*this, m))) {
                                // a consecutive irreversible move with the same king
                                assert(!is_max<Rules>(restricted.moves()));
                                restricted.increment(dest_sq(*this, m));
                        } else {
                                // a first irreversible move with a new king
                                assert(!restricted.moves() || bit::size(active_kings(*this)) > 1);
                                restricted.init(dest_sq(*this, m));
                        }
                        hash_index_ ^= zobrist::hash(std::make_pair(restricted, active_color(*this)));
                }
        }

        void make_passive_king_moves(Move const& m)
        {
                KingMoves& restricted = restricted_[passive_color(*this)];

                if (
                        restricted.moves() && is_capture(*this, m) &&
                        (
                                bit::raw_set_includes(captured_pieces(*this, m), restricted.king()) ||
                                bit::raw_set_includes(captured_pieces(*this, m), passive_pawns(*this))
                        )
                ) {
                        hash_index_ ^= zobrist::hash(std::make_pair(restricted, passive_color(*this)));
                        restricted.reset();
                }
        }

        void make_incremental(Move const& m)
        {
                make_material(m);
                make_to_move();
        }

        void make_material(Move const& m)
        {
                material_ ^= m;
                hash_index_ ^= zobrist::hash(m);
        }

        void make_to_move()
        {
                to_move_ ^= Side::pass;
                hash_index_ ^= zobrist::hash(bool(Side::pass));
        }

        // post-conditions for the constructors and modifiers
        bool material_invariant() const
        {
                return (
                        //material_.invariant() &&
                        bit::raw_set_includes(Board::squares, material().pieces())
                );
        }

        bool hash_index_invariant() const
        {
                return hash_index_ == zobrist::hash(*this);
        }

        // representation
        TreeIterator parent_;
        HashIndex hash_index_;
        Material material_;
        Restricted restricted_;
        PlyCount reversible_moves_;
        PlyCount distance_to_root_;
        bool to_move_;
        BitBoard padding_[6];
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
