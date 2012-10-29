#pragma once
#include <functional>                   // function
#include <dctl/successor/detection/Detector_fwd.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/bit/Bit.hpp>
#include <dctl/board/Compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/Material.hpp>
#include <dctl/node/UnaryProjections.hpp>
#include <dctl/rules/Enum.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves detection
template<bool Color, typename Position, typename Range>
struct detector<Color, Material::pawn, Moves, Position, Range>
:
        public std::function<bool(Position const&)>
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef Compass<Color, Board> Compass;

public:
        bool operator()(Position const& p) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        return select(active_pawns, not_occupied(p));
                else
                        return false;
        }

        bool select(BitBoard active_pawns, BitBoard not_occupied) const
        {
                return branch(active_pawns, not_occupied);
        }

private:
        bool branch(BitBoard active_pawns, BitBoard not_occupied) const
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns, not_occupied) ||
                        parallelize<typename Compass::right_up>(active_pawns, not_occupied)
                );
        }

        template<typename Direction>
        bool parallelize(BitBoard active_pawns, BitBoard not_occupied) const
        {
                return !bit::is_zero(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
