#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for king moves enumeration
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::king, select::Moves, enumeration, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef angle::Compass<Color, Board> Compass;

public:
        template<typename Position>
        static int count(Position const& p)
        {
                return branch(unrestricted_kings(p, Color), not_occupied(p));
        }

private:
        static int branch(BitBoard active_kings, BitBoard not_occupied)
        {
                return (
                        parallelize<typename Compass::left_down >(active_kings, not_occupied) +
                        parallelize<typename Compass::right_down>(active_kings, not_occupied) +
                        parallelize<typename Compass::left_up   >(active_kings, not_occupied) +
                        parallelize<typename Compass::right_up  >(active_kings, not_occupied)
                );
        }

        template<typename Direction>
        static int parallelize(BitBoard active_kings, BitBoard not_occupied)
        {
                return bit::count(
                        Sink<Board, Direction, typename Rules::king_range>()(active_kings, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
