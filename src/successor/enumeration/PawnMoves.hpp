#pragma once
#include "Enumerator_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::pawn, select::Moves, Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;

public:
        static int run(Position const& p)
        {
                if (auto const active_pawns = p.pawns(Color))
                        return select(active_pawns, not_occupied(p));
                else
                        return 0;
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

}       // namespace detail
}       // namespace successor
}       // namespace dctl
