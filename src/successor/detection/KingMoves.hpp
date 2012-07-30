#pragma once
#include "Detector_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves detection
template<bool Color, typename Position>
struct detector<Color, Material::king, select::Moves, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;

public:
        static bool run(Position const& p)
        {
                if (auto const active_kings = unrestricted_kings(p, Color))
                        return branch(active_kings, not_occupied(p));
                else
                        return false;
        }

private:
        static bool branch(BitBoard active_kings, BitBoard not_occupied)
        {
                return (
                        parallelize<typename Compass::left_down >(active_kings, not_occupied) ||
                        parallelize<typename Compass::right_down>(active_kings, not_occupied) ||
                        parallelize<typename Compass::left_up   >(active_kings, not_occupied) ||
                        parallelize<typename Compass::right_up  >(active_kings, not_occupied)
                );
        }

        template<typename Direction>
        static bool parallelize(BitBoard active_kings, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sink<Board, Direction, rules::range::distance_1>()(active_kings, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
