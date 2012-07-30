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
        
// partial specialization for pawn moves detection
template<bool Color, typename Position>
struct detector<Color, Material::pawn, select::Moves, Position>
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;

public:
        bool operator()(Position const& p)
        {
                if (auto const active_pawns = p.pawns(Color))
                        return select(active_pawns, not_occupied(p));
                else
                        return false;
        }

        bool select(BitBoard active_pawns, BitBoard not_occupied)
        {
                return branch(active_pawns, not_occupied);
        }

private:
        bool branch(BitBoard active_pawns, BitBoard not_occupied)
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns, not_occupied) ||
                        parallelize<typename Compass::right_up>(active_pawns, not_occupied)
                );
        }
        
        template<typename Direction>
        bool parallelize(BitBoard active_pawns, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
