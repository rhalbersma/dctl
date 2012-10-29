#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/Enumerator_fwd.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/bit/Bit.hpp>
#include <dctl/board/Compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/Material.hpp>
#include <dctl/node/UnaryProjections.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::king, Moves, Position>
:
        public std::function<int(Position const&)>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef Compass<Color, Board> Compass;

public:
        int operator()(Position const& p) const
        {
                if (auto const active_kings = moveable_kings(p, Color))
                        return select(active_kings, not_occupied(p));
                else
                        return 0;
        }

private:
        int select(BitBoard active_kings, BitBoard not_occupied) const
        {
                return branch(active_kings, not_occupied);
        }

        int branch(BitBoard active_kings, BitBoard not_occupied) const
        {
                return (
                        parallelize<typename Compass::left_down >(active_kings, not_occupied) +
                        parallelize<typename Compass::right_down>(active_kings, not_occupied) +
                        parallelize<typename Compass::left_up   >(active_kings, not_occupied) +
                        parallelize<typename Compass::right_up  >(active_kings, not_occupied)
                );
        }

        template<typename Direction>
        int parallelize(BitBoard active_kings, BitBoard not_occupied) const
        {
                return bit::count(
                        Sink<Board, Direction, typename Rules::king_range>()(active_kings, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
