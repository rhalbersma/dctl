#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
        
// partial specialization for pawn moves detection
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Moves, detection, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef angle::Compass<Color, Board> Compass;

public:
        template<typename Position>
        static bool detect(Position const& p)
        {
                return select(p.pawns(Color), not_occupied(p));
        }

        static bool select(BitBoard active_pawns, BitBoard not_occupied)
        {
                return branch(active_pawns, not_occupied);
        }

private:
        static bool branch(BitBoard active_pawns, BitBoard not_occupied)
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns, not_occupied) ||
                        parallelize<typename Compass::right_up>(active_pawns, not_occupied)
                );
        }
        
        template<typename Direction>
        static bool parallelize(BitBoard active_pawns, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
