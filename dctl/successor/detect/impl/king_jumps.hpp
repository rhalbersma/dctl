#pragma once
#include <dctl/successor/detect/impl/primary_fwd.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king jumps detection
template<bool Color, typename Position, typename Range>
struct detect<Color, Material::king, select::jumps, Position, Range>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;

public:
        bool operator()(Position const& p) const
        {
                return active_kings? branch(active_kings) : false;
        }

private:
        bool branch(BitBoard active_kings) const
        {
                // tag dispatching on king jump directions
                return branch_dispatch(active_kings, typename rules::traits<Rules>::king_jump_directions());
        }

        // overload for kings that capture in the 8 diagonal and orthogonal directions
        bool branch_dispatch(BitBoard active_kings, rules::directions::all) const
        {
                return (
                        branch_dispatch(active_kings, rules::directions::diag()) ||
                        branch_dispatch(active_kings, rules::directions::orth())
                );
        }

        // overload for kings that capture in the 4 diagonal directions
        bool branch_dispatch(BitBoard active_kings, rules::directions::diag) const
        {
                return (
                        parallelize<typename Compass::left_up   >(active_kings) ||
                        parallelize<typename Compass::right_up  >(active_kings) ||
                        parallelize<typename Compass::left_down >(active_kings) ||
                        parallelize<typename Compass::right_down>(active_kings)
                );
        }

        // overload for kings that capture in the 4 orthogonal directions
        bool branch_dispatch(BitBoard active_kings, rules::directions::orth) const
        {
                return (
                        parallelize<typename Compass::left >(active_kings) ||
                        parallelize<typename Compass::right>(active_kings) ||
                        parallelize<typename Compass::up   >(active_kings) ||
                        parallelize<typename Compass::down >(active_kings)
                );
        }

        template<typename Direction>
        bool parallelize(BitBoard active_kings) const
        {
                return !bit::is_zero(
                        Sandwich<Board, Direction, Range>()(active_kings, propagate_.targets(), propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
