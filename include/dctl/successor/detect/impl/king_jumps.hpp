#pragma once
#include <dctl/successor/detect/impl/primary_fwd.hpp>
#include <dctl/pieces/king.hpp>             // king
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>

#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king jumps detection
template<bool Color, typename Position, class Range>
struct detect<Color, pieces::king, select::jumps, Position, Range>
{
private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Compass = board::Compass<Color, Board>;
        using State = Propagate<select::jumps, Position>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit detect(State const& p)
        :
                propagate_(p)
        {}

        // function call operators

        bool operator()(BitBoard active_kings) const
        {
                return active_kings? branch(active_kings) : false;
        }

private:
        bool branch(BitBoard active_kings) const
        {
                // tag dispatching on king jump directions
                return branch_dispatch(active_kings, rules::directions::king_jump<Rules>());
        }

        // overload for kings that jump in the 8 diagonal and orthogonal directions
        bool branch_dispatch(BitBoard active_kings, rules::directions::all) const
        {
                return (
                        branch_dispatch(active_kings, rules::directions::diag()) ||
                        branch_dispatch(active_kings, rules::directions::orth())
                );
        }

        // overload for kings that jump in the 4 diagonal directions
        bool branch_dispatch(BitBoard active_kings, rules::directions::diag) const
        {
                return (
                        parallelize<typename Compass::left_up   >(active_kings) ||
                        parallelize<typename Compass::right_up  >(active_kings) ||
                        parallelize<typename Compass::left_down >(active_kings) ||
                        parallelize<typename Compass::right_down>(active_kings)
                );
        }

        // overload for kings that jump in the 4 orthogonal directions
        bool branch_dispatch(BitBoard active_kings, rules::directions::orth) const
        {
                return (
                        parallelize<typename Compass::left >(active_kings) ||
                        parallelize<typename Compass::right>(active_kings) ||
                        parallelize<typename Compass::up   >(active_kings) ||
                        parallelize<typename Compass::down >(active_kings)
                );
        }

        template<class Direction>
        bool parallelize(BitBoard active_kings) const
        {
                return !bit::empty(
                        Sandwich<Board, Direction, Range>()(active_kings, propagate_.template targets_with_king<Direction>(), propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
