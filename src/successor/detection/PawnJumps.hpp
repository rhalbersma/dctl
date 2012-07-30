#pragma once
#include "Detector_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../node/Targets.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps detection
template<bool Color, typename Position>
struct detector<Color, Material::pawn, select::Jumps, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;

public:
        static bool run(Position const& p)
        {
                if (auto const active_pawns = p.pawns(Color))
                        return branch(active_pawns, targets<Color>(p), not_occupied(p));
                else
                        return false;
        }
        
private:
        static bool branch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // tag dispatching on pawn capture directions
                return branch_dispatch(active_pawns, passive_pieces, not_occupied, typename Rules::pawn_jump_directions());
        }

        // overload for pawns that capture in the 8 diagonal and orthogonal directions
        static bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::all)
        {
                return (
                        branch_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::diag()) ||
                        branch_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::orth())
                );
        }

        // overload for pawns that capture in the 4 diagonal directions
        static bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::diag)
        {
                return (
                        branch_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::up  ()) ||
                        branch_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::down())
                );
        }

        // overload for pawns that capture in the 2 forward diagonal directions
        static bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::up)
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right_up>(active_pawns, passive_pieces, not_occupied)
                );
        }

        // overload for pawns that capture in the 2 backward diagonal directions
        static bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::down)
        {
                return (
                        parallelize<typename Compass::left_down >(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right_down>(active_pawns, passive_pieces, not_occupied)
                );
        }

        // overload for pawns that capture in the 4 orthogonal directions
        static bool branch_dispatch(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::orth)
        {
                return (
                        parallelize<typename Compass::left >(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right>(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::up   >(active_pawns, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::down >(active_pawns, passive_pieces, not_occupied)
                );
        }

        template<typename Direction>
        static bool parallelize(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sandwich<Board, Direction, rules::range::distance_1>()(active_pawns, passive_pieces, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
