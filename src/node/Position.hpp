#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool.hpp>           // false_, true_
#include <functional>                   // function
#include "Position_fwd.hpp"
#include "Move.hpp"
#include "Restricted.hpp"
#include "Side.hpp"
#include "Predicates_fwd.hpp"
#include "../bit/Bit.hpp"
#include "../hash/zobrist/Init.hpp"
#include "../rules/Enum.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template
<
        typename Rules,
        typename Board
>
struct Position
{
public:
        // typedefs

        typedef Rules rules_type;
        typedef Board board_type;

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
                hash_index_ = hash::zobrist::Init<HashIndex, Position>()(*this);
                BOOST_ASSERT(material_invariant());
        }

        // initial position
        static Position initial()
        {
                return Position(Board::initial_mask[Side::black], Board::initial_mask[Side::white], 0, Side::white);
        }

        // queries
        Position const* parent() const
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
        bool active_color() const
        {
                return to_move_;
        }

        // opposite side to move
        bool passive_color() const
        {
                return !to_move_;
        }

        // black or white pawns
        BitBoard pawns(bool color) const
        {
                return material_.pawns(color);
        }

        // black or white kings
        BitBoard kings(bool color) const
        {
                return material_.kings(color);
        }

        // black or white pieces
        BitBoard pieces(bool color) const
        {
                return material_.pieces(color);
        }

        // black and white pawns
        BitBoard pawns() const
        {
                return material_.pawns();
        }

        // black and white kings
        BitBoard kings() const
        {
                return material_.kings();
        }

        // black and white pieces
        BitBoard pieces() const
        {
                return material_.pieces();
        }

        // make a move in a copy from another position
        void copy_make(Position const& other, Move const& move)
        {
                *this = other;          // copy the position
                attach(other);          // attach the position
                make<Rules>(move);      // make the move
        }

        void make(Move const& m)
        {
                BOOST_ASSERT(is_pseudo_legal(*this, m));

                make_irreversible(m);
                make_incremental(m);

                BOOST_ASSERT(material_invariant());
                BOOST_ASSERT(hash_index_invariant());
        }

        void attach(Position const& other)
        {
                parent_ = &other;       // link the pointers
        }

private:
        // implementation
        void detach();

        void make_irreversible(Move const& m)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                make_irreversible(m, typename Rules::is_restricted_same_king_moves());
        }

        // overload for restricted consecutive moves with the same king
        void make_irreversible(Move const& m, boost::mpl::true_)
        {
                make_irreversible(m, boost::mpl::false_());
                make_restricted(m);
        }

        // overload for unrestricted consecutive moves with the same king
        void make_irreversible(Move const& m, boost::mpl::false_)
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
                typedef hash::zobrist::Init<HashIndex, KingMoves> Hash;
                KingMoves& restricted = restricted_[active_color()];

                if (active_kings(*this) && active_pawns(*this)) {
                        hash_index_ ^= Hash()(restricted, active_color());
                        if (is_reversible(*this, m) && !is_max<Rules>(restricted.moves())) {
                                if (restricted.king() & from_sq(*this, m))
                                        restricted.increment(dest_sq(*this, m));
                                else
                                        restricted.init(dest_sq(*this, m));
                                hash_index_ ^= Hash()(restricted, active_color());
                        } else
                                restricted.reset();
                }
        }

        void make_passive_king_moves(Move const& m)
        {
                typedef hash::zobrist::Init<HashIndex, KingMoves> Hash;
                KingMoves& restricted = restricted_[passive_color()];

                if (
                        restricted.moves() && is_capture(*this, m) &&
                        (
                                bit::is_subset_of(restricted.king()   , captured_pieces(*this, m)) ||
                                bit::is_subset_of(passive_pawns(*this), captured_pieces(*this, m))
                        )
                ) {
                        hash_index_ ^= Hash()(restricted, passive_color());
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
                hash_index_ ^= hash::zobrist::Init<HashIndex, Move>()(m);
        }

        void make_to_move()
        {
                to_move_ ^= Side::pass;
                hash_index_ ^= hash::zobrist::Init<HashIndex, bool>()(Side::pass);
        }

        // post-conditions for the constructors and modifiers
        bool material_invariant() const
        {
                return (
                        //material_.invariant() &&
                        bit::is_subset_of(pieces(), Board::squares)
                );
        }

        bool hash_index_invariant() const
        {
                return hash_index_ == hash::zobrist::Init<HashIndex, Position>()(*this);
        }

        // representation
        Position const* parent_;
        HashIndex hash_index_;
        Material material_;
        Restricted restricted_;
        PlyCount reversible_moves_;
        PlyCount distance_to_root_;
        bool to_move_;
        //BitBoard padding_[5];
};

// unoccupied squares
template<typename Rules, typename Board>
BitBoard not_occupied(Position<Rules, Board> const& p)
{
        return Board::squares ^ p.pieces();
}

// pawns for the side to move
template<typename Position>
BitBoard active_pawns(Position const& p)
{
        return p.pawns(p.active_color());
}

// kings for the side to move
template<typename Position>
BitBoard active_kings(Position const& p)
{
        return p.kings(p.active_color());
}

// pieces for the side to move
template<typename Position>
BitBoard active_pieces(Position const& p)
{
        return p.pieces(p.active_color());
}

// pawns for the opposite side
template<typename Position>
BitBoard passive_pawns(Position const& p)
{
        return p.pawns(p.passive_color());
}

// kings for the opposite side
template<typename Position>
BitBoard passive_kings(Position const& p)
{
        return p.kings(p.passive_color());
}

// pieces for the opposite side
template<typename Position>
BitBoard passive_pieces(Position const& p)
{
        return p.pieces(p.passive_color());
}

template<typename Rules, typename Board>
BitBoard moveable_kings(Position<Rules, Board> const& p, bool color)
{
        // tag dispatching on restrictions on consecutive moves with the same king
        return detail::moveable_kings(p, color, typename Rules::is_restricted_same_king_moves());
}

namespace detail {

// overload for unrestricted consecutive moves with the same king
template<typename Rules, typename Board>
BitBoard moveable_kings(Position<Rules, Board> const& p, bool color, boost::mpl::false_)
{
        return p.kings(color);
}

// overload for restricted consecutive moves with the same king
template<typename Rules, typename Board>
BitBoard moveable_kings(Position<Rules, Board> const& p, bool color, boost::mpl::true_)
{
        if (p.kings(color) && p.pawns(color) && is_max<Rules>(p.restricted(color).moves()))
                return p.kings(color) ^ p.restricted(color).king();
        else
                return p.kings(color);
}

}       // namespace detail

template<typename Position>
Position const* grand_parent(Position const& p)
{
        return p.parent() ? p.parent()->parent() : nullptr;
}

template<typename Position>
KingMoves const& active_restricted(Position const& p)
{
        return p.restricted()[p.active_color()];
}

template<typename Position>
KingMoves const& passive_restricted(Position const&);

namespace hash {
namespace zobrist {

// primary template
template<typename Key, typename Index>
struct Init;

// partial specialization for ab initio hashing of positions
template<typename Index, template<typename, typename> class Position, typename Rules, typename Board>
struct Init< Index, Position<Rules, Board> >
:
        std::function<Index(Position<Rules, Board> const&)>
{
        Index operator()(Position<Rules, Board> const& p) const
        {
                return (
                        Init<Index, Material  >()(p.material())     ^
                        Init<Index, bool      >()(p.active_color()) ^
                        Init<Index, Restricted>()(p.restricted())
                );
        }
};

}       // namespace zobrist
}       // namespace hash

}       // namespace dctl
