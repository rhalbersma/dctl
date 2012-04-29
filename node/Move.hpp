#pragma once
#include <array>                        // array
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // equality_comparable, xorable
#include "Move_fwd.hpp"
#include "PiecesInterface.hpp"
#include "Side.hpp"
#include "../bit/Bit.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Int2Type.hpp"

namespace dctl {

template<typename T>
struct Move_
:
        // Curiously Recurring Template Pattern (CRTP)
        public PiecesInterface< T, Move_ >,
        private boost::equality_comparable< Move_<T>, 
        boost::xorable< Move_<T> > >
{
public:
        // structors

        // default constructor
        Move_()
        {
                // no-op
        }

        // zero initialize
        explicit Move_(T /* MUST be zero */)
        {
                init<Side::black>(0, 0, 0);
                BOOST_ASSERT(invariant());
        }

        // initialize with a set of bitboards
        Move_(T black_pieces, T white_pieces, T kings)
        {
                init<Side::black>(black_pieces, white_pieces, kings);
                BOOST_ASSERT(invariant());
        }

        // king move
        template<bool Color>
        static Move_ create(T delta)
        {
                BOOST_ASSERT(pre_condition(delta));
                Move_ tmp;
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
        static Move_ create(T delta, T promotion)
        {
                BOOST_ASSERT(pre_condition(delta, promotion));
                Move_ tmp;
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
        static Move_ create(T delta, T captured_pieces, T captured_kings)
        {
                BOOST_ASSERT(pre_condition<Rules>(delta, captured_pieces, captured_kings));
                Move_ tmp;
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
        static Move_ create(T delta, T promotion, T captured_pieces, T captured_kings)
        {
                BOOST_ASSERT(pre_condition<Rules>(delta, promotion, captured_pieces, captured_kings));
                Move_ tmp;
                tmp.init<Color>(
                        delta,                          // move a pawn between the from and destination squares
                        captured_pieces,                // remove the captured pieces
                        promotion ^ captured_kings      // crown a pawn to a king and remove the captured kings
                );
                BOOST_ASSERT(tmp.pawn_jump_invariant<Rules>(delta, promotion));
                return tmp;
        }

        // modifiers

        // xor-assign the set bits of another piece set
        Move_& operator^=(const Move_& other)
        {
                pieces_[Side::black] ^= other.pieces(Side::black);
                pieces_[Side::white] ^= other.pieces(Side::white);
                kings_ ^= other.kings();
                BOOST_ASSERT(invariant());
                return *this;
        }

        // predicates

        bool operator==(const Move_& other) const
        {
                return (
                        (pieces(Side::black) == other.pieces(Side::black)) &&
                        (pieces(Side::white) == other.pieces(Side::white)) &&
                                    (kings() == other.kings())
                );
        }

private:
        // modifiers

        // initialize with a set of bitboards
        template<bool Color>
        void init(T active_pieces, T passive_pieces, T kings)
        {
                pieces_[ Color] = active_pieces;
                pieces_[!Color] = passive_pieces;
                kings_ = kings;
        }

        // queries

        friend struct PiecesInterface< T, ::dctl::Move_ >;

        // black or white pawns
        T do_pawns(bool color) const
        {
                return do_pieces(color) & ~do_kings();
        }

        // black or white kings
        T do_kings(bool color) const
        {
                return do_pieces(color) & do_kings();
        }

        // black or white pieces
        T do_pieces(bool color) const
        {
                return pieces_[color];
        }

        // black and white pawns
        T do_pawns() const
        {
                return do_pieces() & ~do_kings();
        }

        // black and white kings
        T do_kings() const
        {
                return kings_;
        }

        // black and white pieces
        T do_pieces() const
        {
                return do_pieces(Side::black) ^ do_pieces(Side::white);
        }

        // king move
        static bool pre_condition(T delta)
        {
                return bit::is_double(delta);
        }

        // pawn move
        static bool pre_condition(T delta, T promotion)
        {
                return (
                        bit::is_double(delta) &&
                        !bit::is_multiple(promotion) &&
                        bit::is_subset_of(promotion, delta)
                );
        }

        // king jump
        template<typename Rules>
        static bool pre_condition(T delta, T captured_pieces, T captured_kings)
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
        static bool pre_condition(T delta, T promotion, T captured_pieces, T captured_kings)
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
        bool king_jump_invariant(T delta, T captured_pieces) const
        {
                return (
                        (side_invariant() || is_intersecting_capture<Rules>(delta, captured_pieces)) &&
                        material_invariant()
                );
        }

        // logical consistency of a pawn jump
        template<typename Rules>
        bool pawn_jump_invariant(T delta, T promotion) const
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

        // representation

        std::array<T, 2> pieces_;       // black and white pieces
        T kings_;                       // kings
};

template<typename Rules, typename T>
bool is_intersecting_capture(T delta, T captured_pieces)
{
        // tag dispatching on capture removal
        return aux::is_intersecting_capture(
                delta, captured_pieces,
                Int2Type<rules::jump_removal<Rules>::value>()
        );
}

template<typename Rules, typename T>
bool is_intersecting_promotion(T promotion, T delta)
{
        // tag dispatching on promotion condition
        return aux::is_intersecting_promotion(
                promotion, delta,
                Int2Type<rules::promotion_condition<Rules>::value>()
        );
}

namespace aux {

// specialization for apres-fini capture removal
template<typename T>
bool is_intersecting_capture(
        T /* delta */, T /* captured_pieces */,
        Int2Type<rules::remove_af>
)
{
        return false;
}

// specialization for en-passant capture removal
template<typename T>
bool is_intersecting_capture(
        T delta, T captured_pieces,
        Int2Type<rules::remove_ep>
)
{
        // [FEN "W:WK25:B8,9,20,23,24"] (Thai draughts)
        // white has to capture 25x20, landing on a square it also captured on
        return bit::is_single(delta & captured_pieces) && bit::is_multiple(captured_pieces);
}

// specialization for apres-fini promotion
template<typename T>
bool is_intersecting_promotion(
        T /* promotion */, T /* delta */,
        Int2Type<rules::promote_af>
)
{
        return false;
}

// specialization for en-passant promotion
template<typename T>
bool is_intersecting_promotion(
        T promotion, T delta,
        Int2Type<rules::promote_ep>
)
{
        // [FEN "W:W15:B10,13,20,23"] (Russian draughts)
        // white has to capture 15x15, promoting on its original square
        return bit::is_single(promotion) && bit::is_zero(delta);
}

}       // namespace aux
}       // namespace dctl
