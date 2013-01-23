#pragma once
#include <dctl/successor/detection/detector_fwd.hpp>
#include <dctl/successor/select.hpp>
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

// partial specialization for king jumps detection
template<bool Color, typename Position, typename Range>
struct detector<Color, Material::king, Jumps, Position, Range>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;

public:
        bool operator()(Position const& p) const
        {
                auto const active_kings = p.kings(Color);
                return active_kings? select(active_kings, p.pieces(!Color), not_occupied(p)) : false;
        }

private:
        bool select(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied) const
        {
                return branch(active_kings, passive_pieces, not_occupied);
        }

        bool branch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied) const
        {
                // tag dispatching on king jump directions
                return branch_dispatch(active_kings, passive_pieces, not_occupied, typename rules::traits<Rules>::king_jump_directions());
        }

        // overload for kings that capture in the 8 diagonal and orthogonal directions
        bool branch_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::all) const
        {
                return (
                        branch_dispatch(active_kings, passive_pieces, not_occupied, rules::directions::diag()) ||
                        branch_dispatch(active_kings, passive_pieces, not_occupied, rules::directions::orth())
                );
        }

        // overload for kings that capture in the 4 diagonal directions
        bool branch_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::diag) const
        {
                return (
                        parallelize<typename Compass::left_up   >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right_up  >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::left_down >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right_down>(active_kings, passive_pieces, not_occupied)
                );
        }

        // overload for kings that capture in the 4 orthogonal directions
        bool branch_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::orth) const
        {
                return (
                        parallelize<typename Compass::left >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right>(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::up   >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::down >(active_kings, passive_pieces, not_occupied)
                );
        }

        template<typename Direction>
        bool parallelize(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied) const
        {
                return !bit::is_zero(
                        Sandwich<Board, Direction, Range>()(active_kings, passive_pieces, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
