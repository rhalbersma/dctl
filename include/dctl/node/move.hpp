#pragma once
#include <cassert>                      // assert
#include <type_traits>
#include <tuple>
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
        using board_type = Board;
        using rules_type = Rules;
        using T = typename Board::bit_type;

        // structors

        // default constructor
        Move() = default;

        // king move
        Move(int from, int dest)
        :
                from_{from},
                dest_{dest}
        {
                //assert(pre_condition(delta));
                //assert(invariant());
        }

        // pawn move
        Move(int from, int dest, bool promotion)
        :
                from_{from},
                dest_{dest},
                promotion_{promotion}
        {
                //assert(pre_condition(delta, promotion));
                //assert(tmp.invariant());
        }

        // king jump
        Move(T captured_pieces, T captured_kings, int from, int dest)
        :
                captured_pieces_{captured_pieces},
                captured_kings_{captured_kings},
                from_{from},
                dest_{dest}
        {
                //assert(pre_condition(delta, captured_pieces, captured_kings));
                //assert(tmp.king_jump_invariant(delta, captured_pieces));
        }

        // pawn jump
        Move(T captured_pieces, T captured_kings, int from, int dest, bool promotion)
        :
                captured_pieces_{captured_pieces},
                captured_kings_{captured_kings},
                from_{from},
                dest_{dest},
                promotion_{promotion}

        {
                //assert(pre_condition(delta, promotion, captured_pieces, captured_kings));
                //assert(tmp.pawn_jump_invariant(delta, promotion));
        }

        // predicates

        // operator!= provided by boost::equality_comparable1
        friend bool operator==(Move const& lhs, Move const& rhs)
        {
                return
                        std::tie(lhs.captured_pieces_, lhs.captured_kings_, lhs.from_, lhs.dest_, lhs.promotion_) ==
                        std::tie(rhs.captured_pieces_, rhs.captured_kings_, rhs.from_, rhs.dest_, rhs.promotion_)
                ;
        }

        // queries

        auto captured_pieces() const noexcept
        {
                return captured_pieces_;
        }

        auto captured_kings() const noexcept
        {
                return captured_kings_;
        }

        auto from() const noexcept
        {
                return from_;
        }

        auto dest() const noexcept
        {
                return dest_;
        }

        auto promotion() const noexcept
        {
                return promotion_;
        }

private:
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

        // logical consistency of a king jump
        bool king_jump_invariant(T delta, T captured_pieces) const
        {
                return (
                        is_intersecting_capture<Rules>(delta, captured_pieces)
                );
        }

        // logical consistency of a pawn jump
        bool pawn_jump_invariant(T delta, T promotion) const
        {
                return (
                        is_intersecting_promotion<Rules>(promotion, delta)
                );
        }

        // representation

        T captured_pieces_{};
        T captured_kings_{};
        int from_{};
        int dest_{};
        bool promotion_{};
};

}       // namespace dctl
