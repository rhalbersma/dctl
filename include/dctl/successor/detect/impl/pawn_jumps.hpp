#pragma once
#include <dctl/successor/detect/impl/primary_fwd.hpp>
#include <dctl/pieces/pawn.hpp>             // pawn
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>

#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/targets.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for pawn jumps detection
template<bool Color, typename Position, class Range>
struct detect<Color, pieces::pawn, select::jumps, Position, Range>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;
        typedef Propagate<select::jumps, Position> State;

        // representation

        State& propagate_;

public:
        // structors

        explicit detect(State& p)
        :
                propagate_(p)
        {}

        // function call operators

        bool operator()(BitBoard active_pawns) const
        {
                return active_pawns? branch(active_pawns) : false;
        }

private:
        bool branch(BitBoard active_pawns) const
        {
                // tag dispatching on pawn jump directions
                return branch_dispatch(active_pawns, rules::directions::pawn_jump<Rules>());
        }

        // overload for pawns that jump in the 8 diagonal and orthogonal directions
        bool branch_dispatch(BitBoard active_pawns, rules::directions::all) const
        {
                return (
                        branch_dispatch(active_pawns, rules::directions::diag()) ||
                        branch_dispatch(active_pawns, rules::directions::orth())
                );
        }

        // overload for pawns that jump in the 4 diagonal directions
        bool branch_dispatch(BitBoard active_pawns, rules::directions::diag) const
        {
                return (
                        branch_dispatch(active_pawns, rules::directions::up  ()) ||
                        branch_dispatch(active_pawns, rules::directions::down())
                );
        }

        // overload for pawns that jump in the 2 forward diagonal directions
        bool branch_dispatch(BitBoard active_pawns, rules::directions::up) const
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns) ||
                        parallelize<typename Compass::right_up>(active_pawns)
                );
        }

        // overload for pawns that jump in the 2 backward diagonal directions
        bool branch_dispatch(BitBoard active_pawns, rules::directions::down) const
        {
                return (
                        parallelize<typename Compass::left_down >(active_pawns) ||
                        parallelize<typename Compass::right_down>(active_pawns)
                );
        }

        // overload for pawns that jump in the 4 orthogonal directions
        bool branch_dispatch(BitBoard active_pawns, rules::directions::orth) const
        {
                return (
                        parallelize<typename Compass::left >(active_pawns) ||
                        parallelize<typename Compass::right>(active_pawns) ||
                        parallelize<typename Compass::up   >(active_pawns) ||
                        parallelize<typename Compass::down >(active_pawns)
                );
        }

        template<class Direction>
        bool parallelize(BitBoard active_pawns) const
        {
                return !bit::empty(
                        Sandwich<Board, Direction, rules::range::distance_1>()(active_pawns, propagate_.template targets_with_pawn<Direction>(), propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
