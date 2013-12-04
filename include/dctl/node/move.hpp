#pragma once
#include <cassert>                      // assert
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>          // equality_comparable, xorable
#include <dctl/node/move_fwd.hpp>
#include <dctl/node/side.hpp>
#include <dctl/rules/traits.hpp>

#include <dctl/bit/algorithm.hpp>
#include <dctl/bit/predicates.hpp>

namespace dctl {
namespace detail {

// overload for apres-fini capture removal
template<class T>
bool is_intersecting_capture(T /* delta */, T /* captured_pieces */, rules::phase::apres_fini)
{
        return false;
}

// overload for en-passant capture removal
template<class T>
bool is_intersecting_capture(T delta, T captured_pieces, rules::phase::en_passant)
{
        // [FEN "W:WK25:B8,9,20,23,24"] (Thai draughts)
        // white has to capture 25x20, landing on a square it also captured on
        return bit::is_single(delta & captured_pieces) && bit::is_multiple(captured_pieces);
}

// overload for apres-fini promotion
template<class T>
bool is_intersecting_promotion(T /* promotion */, T /* delta */, rules::phase::apres_fini)
{
        return false;
}

// overload for en-passant promotion
template<class T>
bool is_intersecting_promotion(T promotion, T delta, rules::phase::en_passant)
{
        // [FEN "W:W15:B10,13,20,23"] (Russian draughts)
        // white has to capture 15x15, promoting on its original square
        return bit::is_single(promotion) && delta.empty();
}

}       // namespace detail

template<class Rules, class T>
bool is_intersecting_capture(T delta, T captured_pieces)
{
        // tag dispatching on capture removal
        return detail::is_intersecting_capture(delta, captured_pieces, rules::phase::capture<Rules>{});
}

template<class Rules, class T>
bool is_intersecting_promotion(T promotion, T delta)
{
        // tag dispatching on pawn promotion
        return detail::is_intersecting_promotion(promotion, delta, rules::phase::promotion<Rules>{});
}

template<class Rules, class Board>
struct Move
        // http://www.boost.org/doc/libs/1_51_0/libs/utility/operators.htm#chaining
        // use base class chaining to ensure Empty Base Optimization
:       boost::equality_comparable1< Move<Rules, Board>
,       boost::xorable1< Move<Rules, Board>
        > >
{
public:
        using T = typename Board::bit_type;

        // structors

        // default constructor
        Move() = default;

        // zero initialize
        explicit Move(T /* MUST be zero */)
        {
                init<Side::black>(T{}, T{}, T{});
                assert(invariant());
        }

        // initialize with a set of bitboards
        Move(T black_pieces, T white_pieces, T kings)
        {
                init<Side::black>(black_pieces, white_pieces, kings);
                assert(invariant());
        }

        // king move
        template<bool Color>
        static Move create(std::pair<int, int> const& from_dest)
        {
                auto const delta = T{from_dest.first, from_dest.second};
                assert(pre_condition(delta));
                Move tmp;
                tmp.template init<Color>(
                        delta,  // move a king between the from and destination squares
                        T{},
                        delta   // move a king between the from and destination squares
                );
                assert(tmp.invariant());
                return tmp;
        }

        // pawn move
        template<bool Color>
        static Move create(std::pair<int, int> const& from_dest, bool is_promotion)
        {
                auto const delta = T{from_dest.first, from_dest.second};
                auto const promotion = is_promotion? T{from_dest.second} : T{};
                assert(pre_condition(delta, promotion));
                Move tmp;
                tmp.template init<Color>(
                        delta,          // move a pawn between the from and destination squares
                        T{},
                        promotion       // crown a pawn to a king
                );
                assert(tmp.invariant());
                return tmp;
        }

        // king jump
        template<bool Color>
        static Move create(std::pair<int, int> const& from_dest, T captured_pieces, T captured_kings)
        {
                auto const delta = (from_dest.first == from_dest.second)? T{} : T{from_dest.first, from_dest.second};
                assert(pre_condition(delta, captured_pieces, captured_kings));
                Move tmp;
                tmp.template init<Color>(
                        delta,                  // move a king between the from and destination square
                        captured_pieces,        // remove the captured pieces
                        delta ^ captured_kings  // move a king and remove the captured kings
                );
                assert(tmp.king_jump_invariant(delta, captured_pieces));
                return tmp;
        }

        // pawn jump
        template<bool Color>
        static Move create(std::pair<int, int> const& from_dest, bool is_promotion, T captured_pieces, T captured_kings)
        {
                auto const delta = (from_dest.first == from_dest.second)? T{} : T{from_dest.first, from_dest.second};
                auto const promotion = is_promotion? T{from_dest.second} : T{};
                assert(pre_condition(delta, promotion, captured_pieces, captured_kings));
                Move tmp;
                tmp.template init<Color>(
                        delta,                          // move a pawn between the from and destination squares
                        captured_pieces,                // remove the captured pieces
                        promotion ^ captured_kings      // crown a pawn to a king and remove the captured kings
                );
                assert(tmp.pawn_jump_invariant(delta, promotion));
                return tmp;
        }

        // predicates

        // operator!= provided by boost::equality_comparable1
        friend bool operator==(Move const& lhs, Move const& rhs)
        {
                return (
                        (lhs.pieces(Side::black) == rhs.pieces(Side::black)) &&
                        (lhs.pieces(Side::white) == rhs.pieces(Side::white)) &&
                                    (lhs.kings() == rhs.kings())
                );
        }

        // queries

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
                        bit::set_includes(delta, promotion)
                );
        }

        // king jump
        static bool pre_condition(T delta, T captured_pieces, T captured_kings)
        {
                return (
                        (bit::is_double(delta) || delta.empty()) &&
                        !captured_pieces.empty() &&
                        (
                                bit::set_exclusive(delta, captured_pieces) ||

                                // EXCEPTION: for intersecting captures, delta overlaps with captured pieces
                                is_intersecting_capture<Rules>(delta, captured_pieces)
                        ) &&
                        bit::set_includes(captured_pieces, captured_kings)
                );
        }

        // pawn jump
       static bool pre_condition(T delta, T promotion, T captured_pieces, T captured_kings)
        {
                return (
                        (bit::is_double(delta) || delta.empty()) &&
                        !bit::is_multiple(promotion) &&
                        !captured_pieces.empty() &&
                        bit::set_exclusive(delta, captured_pieces) &&
                        (
                                bit::set_includes(delta, promotion) ||

                                // EXCEPTION: for intersecting promotions, delta is empty, and promotion is non-empty
                                is_intersecting_promotion<Rules>(promotion, delta)
                        ) &&
                        bit::set_includes(captured_pieces, captured_kings)
                );
        }

        // logical consistency of the representation
        bool invariant() const
        {
                return side_invariant() && material_invariant();
        }

        // logical consistency of a king jump
        bool king_jump_invariant(T delta, T captured_pieces) const
        {
                return (
                        (side_invariant() || is_intersecting_capture<Rules>(delta, captured_pieces)) &&
                        material_invariant()
                );
        }

        // logical consistency of a pawn jump
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
                return bit::set_exclusive(pieces(Side::black), pieces(Side::white));
        }

        // kings are a subset of pieces
        bool material_invariant() const
        {
                return bit::set_includes(pieces(), kings());
        }

        // representation

        T pieces_[2];   // black and white pieces
        T kings_;       // kings
};

}       // namespace dctl
