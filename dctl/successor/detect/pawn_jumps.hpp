#pragma once
#include <dctl/successor/detect/detector_fwd.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/targets.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps detection
template<bool Color, typename Position, typename Range>
struct detector<Color, Material::pawn, select::jumps, Position, Range>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;

public:
        bool operator()(Position const& p) const
        {
                auto const active_pawns = p.pawns(Color);
                return active_pawns? select(active_pawns, targets<Color>(p), not_occupied(p)) : false;
        }

        bool select(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied) const
        {
                return branch(active_pawns, passive_pieces, not_occupied);
        }

private:
        bool branch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied) const
        {
                // tag dispatching on pawn jump directions
                return branch_dispatch(active_pawns, passive_pieces, not_occupied, typename rules::traits<Rules>::pawn_jump_directions());
        }

        // overload for pawns that capture in the 8 diagonal and orthogonal directions
        bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::all) const
        {
                return (
                        branch_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::diag()) ||
                        branch_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::orth())
                );
        }

        // overload for pawns that capture in the 4 diagonal directions
        bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::diag) const
        {
                return (
                        branch_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::up  ()) ||
                        branch_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::down())
                );
        }

        // overload for pawns that capture in the 2 forward diagonal directions
        bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::up) const
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right_up>(active_pawns, passive_pieces, not_occupied)
                );
        }

        // overload for pawns that capture in the 2 backward diagonal directions
        bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::down) const
        {
                return (
                        parallelize<typename Compass::left_down >(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right_down>(active_pawns, passive_pieces, not_occupied)
                );
        }

        // overload for pawns that capture in the 4 orthogonal directions
        bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::orth) const
        {
                return (
                        parallelize<typename Compass::left >(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right>(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::up   >(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::down >(active_pawns, passive_pieces, not_occupied)
                );
        }

        template<typename Direction>
        bool parallelize(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied) const
        {
                return !bit::is_zero(
                        Sandwich<Board, Direction, rules::range::distance_1>()(active_pawns, passive_pieces, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
