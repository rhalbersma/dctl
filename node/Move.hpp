#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Move_fwd.hpp"
#include "Side.hpp"
#include "../bit/Bit.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Int2Type.hpp"

namespace dctl {

struct Move
{
public:
        // default constructor
        Move()
        {
                // no-op
        }

        // zero initialize
        explicit Move(BitBoard /* MUST be zero */)
        {
                init<Side::black>(0, 0, 0);
                BOOST_ASSERT(invariant());
        }

        // initialize with a set of bitboards
        Move(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings)
        {
                init<Side::black>(black_pieces, white_pieces, kings);
                BOOST_ASSERT(invariant());
        }

        // king move
        template<bool Color>
        static Move create(BitBoard delta)
        {
                BOOST_ASSERT(pre_condition(delta));
                Move tmp;
                tmp.init<Color>(
                        delta,  // move a king between the from and destination squares
                        0,
                        delta   // move a king between the from and destination squares
                );
                BOOST_ASSERT(tmp.invariant());
                return tmp;
        }

        // pawn move
        template<bool Color>
        static Move create(BitBoard delta, BitBoard promotion)
        {
                BOOST_ASSERT(pre_condition(delta, promotion));
                Move tmp;
                tmp.init<Color>(
                        delta,          // move a pawn between the from and destination squares
                        0,
                        promotion       // crown a pawn to a king
                );
                BOOST_ASSERT(tmp.invariant());
                return tmp;
        }

        // king jump
        template<bool Color, typename Rules>
        static Move create(BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings)
        {
                BOOST_ASSERT(pre_condition<Rules>(delta, captured_pieces, captured_kings));
                Move tmp;
                tmp.init<Color>(
                        delta,                  // move a king between the from and destination square
                        captured_pieces,        // remove the captured pieces
                        delta ^ captured_kings  // move a king and remove the captured kings
                );
                BOOST_ASSERT(tmp.king_jump_invariant<Rules>(delta, captured_pieces));
                return tmp;
        }

        // pawn jump
        template<bool Color, typename Rules>
        static Move create(BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
        {
                BOOST_ASSERT(pre_condition<Rules>(delta, promotion, captured_pieces, captured_kings));
                Move tmp;
                tmp.init<Color>(
                        delta,                          // move a pawn between the from and destination squares
                        captured_pieces,                // remove the captured pieces
                        promotion ^ captured_kings      // crown a pawn to a king and remove the captured kings
                );
                BOOST_ASSERT(tmp.pawn_jump_invariant<Rules>(delta, promotion));
                return tmp;
        }

        // black or white pawns
        BitBoard pawns(bool color) const
        {
                return pieces(color) & ~kings();
        }

        // black or white kings
        BitBoard kings(bool color) const
        {
                return pieces(color) & kings();
        }

        // black or white pieces
        BitBoard pieces(bool color) const
        {
                return pieces_[color];
        }

        // black and white pawns
        BitBoard pawns() const
        {
                return pieces() & ~kings();
        }

        // black and white kings
        BitBoard kings() const
        {
                return kings_;
        }

        // black and white pieces
        BitBoard pieces() const
        {
                return pieces(Side::black) ^ pieces(Side::white);
        }

        // xor-assign the set bits of another piece set
        Move& operator^=(const Move& other)
        {
                pieces_[Side::black] ^= other.pieces(Side::black);
                pieces_[Side::white] ^= other.pieces(Side::white);
                kings_ ^= other.kings();
                BOOST_ASSERT(invariant());
                return *this;
        }

private:
        // king move
        static bool pre_condition(BitBoard delta)
        {
                return bit::is_double(delta);
        }

        // pawn move
        static bool pre_condition(BitBoard delta, BitBoard promotion)
        {
                return (
                        bit::is_double(delta) &&
                        !bit::is_multiple(promotion) &&
                        bit::is_subset_of(promotion, delta)
                );
        }

        // king jump
        template<typename Rules>
        static bool pre_condition(BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings)
        {
                return (
                        (bit::is_double(delta) || bit::is_zero(delta)) &&
                        !bit::is_zero(captured_pieces) &&
                        (
                                bit::is_exclusive(delta, captured_pieces) ||

                                // EXCEPTION: for intersecting captures, delta overlaps with captured pieces
                                is_intersecting_capture<Rules>(delta, captured_pieces)
                        ) &&
                        bit::is_subset_of(captured_kings, captured_pieces)
                );
        }

        // pawn jump
        template<typename Rules>
        static bool pre_condition(BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
        {
                return (
                        (bit::is_double(delta) || bit::is_zero(delta)) &&
                        !bit::is_multiple(promotion) &&
                        !bit::is_zero(captured_pieces) &&
                        bit::is_exclusive(delta, captured_pieces) &&
                        (
                                bit::is_subset_of(promotion, delta) ||

                                // EXCEPTION: for intersecting promotions, delta is empty, and promotion is non-empty
                                is_intersecting_promotion<Rules>(promotion, delta)
                        ) &&
                        bit::is_subset_of(captured_kings, captured_pieces)
                );
        }

        // logical consistency of the representation
        bool invariant() const
        {
                return side_invariant() && material_invariant();
        }

        // logical consistency of a king jump
        template<typename Rules>
        bool king_jump_invariant(BitBoard delta, BitBoard captured_pieces) const
        {
                return (
                        (side_invariant() || is_intersecting_capture<Rules>(delta, captured_pieces)) &&
                        material_invariant()
                );
        }

        // logical consistency of a pawn jump
        template<typename Rules>
        bool pawn_jump_invariant(BitBoard delta, BitBoard promotion) const
        {
                return (
                        side_invariant() &&
                        (material_invariant() || is_intersecting_promotion<Rules>(promotion, delta))
                );
        }

        // black and white pieces are mutually exclusive
        bool side_invariant() const
        {
                return bit::is_exclusive(pieces(Side::black), pieces(Side::white));
        }

        // kings are a subset of pieces
        bool material_invariant() const
        {
                return bit::is_subset_of(kings(), pieces());
        }

        // initialize with a set of bitboards
        template<bool Color>
        void init(BitBoard active_pieces, BitBoard passive_pieces, BitBoard kings)
        {
                pieces_[ Color] = active_pieces;
                pieces_[!Color] = passive_pieces;
                kings_ = kings;
        }

        // representation
        BitBoard pieces_[2];    // black and white pieces
        BitBoard kings_;        // kings
};

// xor the set bits of two piece sets
inline
Move operator^(const Move& left, const Move& right)
{
        return Move(left) ^= right;
}

inline
bool operator==(const Move& left, const Move& right)
{
        return (
                (left.pieces(Side::black) == right.pieces(Side::black)) &&
                (left.pieces(Side::white) == right.pieces(Side::white)) &&
                            (left.kings() == right.kings())
        );
}

inline
bool operator!=(const Move& left, const Move& right)
{
        return !(left == right);
}

template<typename Rules>
bool is_intersecting_capture(BitBoard delta, BitBoard captured_pieces)
{
        // tag dispatching on capture removal
        return aux::is_intersecting_capture(
                delta, captured_pieces,
                Int2Type<rules::capture_removal<Rules>::value>()
        );
}

template<typename Rules>
bool is_intersecting_promotion(BitBoard promotion, BitBoard delta)
{
        // tag dispatching on promotion condition
        return aux::is_intersecting_promotion(
                promotion, delta,
                Int2Type<rules::promotion_condition<Rules>::value>()
        );
}

namespace aux {

// specialization for apres-fini capture removal
inline
bool is_intersecting_capture(BitBoard /* delta */, BitBoard /* captured_pieces */,
        Int2Type<rules::remove_af>
)
{
        return false;
}

// specialization for en-passant capture removal
inline
bool is_intersecting_capture(BitBoard delta, BitBoard captured_pieces,
        Int2Type<rules::remove_ep>
)
{
        // [FEN "W:WK25:B8,9,20,23,24"] (Thai draughts)
        // white has to capture 25x20, landing on a square it also captured on
        return bit::is_single(delta & captured_pieces) && bit::is_multiple(captured_pieces);
}

// specialization for apres-fini promotion
inline
bool is_intersecting_promotion(BitBoard /* promotion */, BitBoard /* delta */,
        Int2Type<rules::promote_af>
)
{
        return false;
}

// specialization for en-passant promotion
inline
bool is_intersecting_promotion(BitBoard promotion, BitBoard delta,
        Int2Type<rules::promote_ep>
)
{
        // [FEN "W:W15:B10,13,20,23"] (Russian draughts)
        // white has to capture 15x15, promoting on its original square
        return bit::is_single(promotion) && bit::is_zero(delta);
}

}       // namespace aux
}       // namespace dctl
