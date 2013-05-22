#pragma once
#include <type_traits>
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/identity.hpp>       // identity
#include <boost/operators.hpp>          // equality_comparable, xorable
#include <dctl/bit/bit.hpp>
#include <dctl/node/move_fwd.hpp>
#include <dctl/node/i_pieces.hpp>
#include <dctl/node/side.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>
#include <dctl/packed/algorithm.hpp>

namespace dctl {
namespace detail {

// overload for apres-fini capture removal
template<typename T>
bool is_intersecting_capture(T /* delta */, T /* captured_pieces */, rules::removal::apres_fini)
{
        return false;
}

// overload for en-passant capture removal
template<typename T>
bool is_intersecting_capture(T delta, T captured_pieces, rules::removal::en_passant)
{
        // [FEN "W:WK25:B8,9,20,23,24"] (Thai draughts)
        // white has to capture 25x20, landing on a square it also captured on
        return bit::is_single(delta & captured_pieces) && bit::is_multiple(captured_pieces);
}

// overload for apres-fini promotion
template<typename T>
bool is_intersecting_promotion(T /* promotion */, T /* delta */, rules::promotion::apres_fini)
{
        return false;
}

// overload for en-passant promotion
template<typename T>
bool is_intersecting_promotion(T promotion, T delta, rules::promotion::en_passant)
{
        // [FEN "W:W15:B10,13,20,23"] (Russian draughts)
        // white has to capture 15x15, promoting on its original square
        return bit::is_single(promotion) && bit::is_zero(delta);
}

}       // namespace detail

template<typename Rules, typename T>
bool is_intersecting_capture(T delta, T captured_pieces)
{
        // tag dispatching on capture removal
        return detail::is_intersecting_capture(delta, captured_pieces, rules::captures_removal<Rules>());
}

template<typename Rules, typename T>
bool is_intersecting_promotion(T promotion, T delta)
{
        // tag dispatching on pawn promotion
        return detail::is_intersecting_promotion(promotion, delta, rules::pawn_promotion<Rules>());
}

template<typename T>
struct Move_
        // http://www.boost.org/doc/libs/1_51_0/libs/utility/operators.htm#chaining
        // use base class chaining to ensure Empty Base Optimization
:       boost::equality_comparable1< Move_<T>
,       boost::xorable1< Move_<T>
,       IPieces< Move_, T >
        > >
{
        friend class IPieces< ::dctl::Move_, T >;

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
                tmp.template init<Color>(
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
                tmp.template init<Color>(
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
                tmp.template init<Color>(
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
                tmp.template init<Color>(
                        delta,                          // move a pawn between the from and destination squares
                        captured_pieces,                // remove the captured pieces
                        promotion ^ captured_kings      // crown a pawn to a king and remove the captured kings
                );
                BOOST_ASSERT(tmp.pawn_jump_invariant<Rules>(delta, promotion));
                return tmp;
        }

        // modifiers

        // xor-assign the set bits of another piece set
        // operator^ provided by boost::xorable1
        Move_& operator^=(Move_ const& other)
        {
                pieces_[Side::black] ^= other.pieces(Side::black);
                pieces_[Side::white] ^= other.pieces(Side::white);
                kings_ ^= other.kings();
                BOOST_ASSERT(invariant());
                return *this;
        }

        // predicates

        // operator!= provided by boost::equality_comparable1
        friend bool operator==(Move_ const& lhs, Move_ const& rhs)
        {
                return (
                        (lhs.pieces(Side::black) == rhs.pieces(Side::black)) &&
                        (lhs.pieces(Side::white) == rhs.pieces(Side::white)) &&
                                    (lhs.kings() == rhs.kings())
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
                        packed::set_includes(delta, promotion)
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
                                packed::set_exclusive(delta, captured_pieces) ||

                                // EXCEPTION: for intersecting captures, delta overlaps with captured pieces
                                is_intersecting_capture<Rules>(delta, captured_pieces)
                        ) &&
                        packed::set_includes(captured_pieces, captured_kings)
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
                        packed::set_exclusive(delta, captured_pieces) &&
                        (
                                packed::set_includes(delta, promotion) ||

                                // EXCEPTION: for intersecting promotions, delta is empty, and promotion is non-empty
                                is_intersecting_promotion<Rules>(promotion, delta)
                        ) &&
                        packed::set_includes(captured_pieces, captured_kings)
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
                return packed::set_exclusive(this->pieces(Side::black), this->pieces(Side::white));
        }

        // kings are a subset of pieces
        bool material_invariant() const
        {
                return packed::set_includes(this->pieces(), this->kings());
        }

        // representation

        T pieces_[2];   // black and white pieces
        T kings_;       // kings
};

}       // namespace dctl
