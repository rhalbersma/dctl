#pragma once
#include "Detector_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps detection
template<bool Color, typename Position>
struct detector<Color, Material::king, select::Jumps, Position>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;
        
public:
        bool operator()(Position const& p)
        {
                if (auto const active_kings = p.kings(Color))
                        return branch(active_kings, p.pieces(!Color), not_occupied(p));
                else
                        return false;
        }
        
private:
        bool branch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // tag dispatching on king capture directions
                return branch_dispatch(active_kings, passive_pieces, not_occupied, typename Rules::king_jump_directions());
        }

        // overload for kings that capture in the 8 diagonal and orthogonal directions
        bool branch_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::all)
        {
                return (
                        branch_dispatch(active_kings, passive_pieces, not_occupied, rules::directions::diag()) ||
                        branch_dispatch(active_kings, passive_pieces, not_occupied, rules::directions::orth())
                );
        }

        // overload for kings that capture in the 4 diagonal directions
        bool branch_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::diag)
        {
                return (
                        parallelize<typename Compass::left_up   >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right_up  >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::left_down >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right_down>(active_kings, passive_pieces, not_occupied)
                );
        }

        // overload for kings that capture in the 4 orthogonal directions
        bool branch_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::orth)
        {
                return (
                        parallelize<typename Compass::left >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::right>(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::up   >(active_kings, passive_pieces, not_occupied) ||
                        parallelize<typename Compass::down >(active_kings, passive_pieces, not_occupied)
                );
        }

        template<typename Direction>
        bool parallelize(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // partial specialiations of Sandwich for king range
                return !bit::is_zero(
                        Sandwich<Board, Direction, typename Rules::king_range>()
                        (active_kings, passive_pieces, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
