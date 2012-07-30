#pragma once
#include "Enumerator_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::king, select::Moves, Position>
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
        static int run(Position const& p)
        {
                if (auto const active_kings = unrestricted_kings(p, Color))
                        return branch(active_kings, not_occupied(p));
                else
                        return 0;
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

}       // namespace detail
}       // namespace successor
}       // namespace dctl
