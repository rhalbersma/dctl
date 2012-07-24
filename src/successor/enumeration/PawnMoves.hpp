#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for pawn moves enumeration
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Moves, enumeration, Rules, Board>
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
                return select(p.pawns(Color), not_occupied(p));
        }

        static int select(BitBoard active_pawns, BitBoard not_occupied)
        {
                return branch(active_pawns, not_occupied);
        }

private:
        static int branch(BitBoard active_pawns, BitBoard not_occupied)
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns, not_occupied) +
                        parallelize<typename Compass::right_up>(active_pawns, not_occupied)
                );
        }

        template<typename Direction>
        static int parallelize(BitBoard active_pawns, BitBoard not_occupied)
        {
                return bit::count(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
