#pragma once
#include <functional>                   // function
#include <dctl/successor/detection/detector_fwd.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves detection
template<bool Color, typename Position, typename Range>
struct detector<Color, Material::king, Moves, Position, Range>
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;

public:
        bool operator()(Position const& p) const
        {
                if (auto const active_kings = moveable_kings(p, Color))
                        return select(active_kings, not_occupied(p));
                else
                        return false;
        }

private:
        bool select(BitBoard active_kings, BitBoard not_occupied) const
        {
                return branch(active_kings, not_occupied);
        }

        bool branch(BitBoard active_kings, BitBoard not_occupied) const
        {
                return (
                        parallelize<typename Compass::left_down >(active_kings, not_occupied) ||
                        parallelize<typename Compass::right_down>(active_kings, not_occupied) ||
                        parallelize<typename Compass::left_up   >(active_kings, not_occupied) ||
                        parallelize<typename Compass::right_up  >(active_kings, not_occupied)
                );
        }

        template<typename Direction>
        bool parallelize(BitBoard active_kings, BitBoard not_occupied) const
        {
                return !bit::is_zero(
                        Sink<Board, Direction, rules::range::distance_1>()(active_kings, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
