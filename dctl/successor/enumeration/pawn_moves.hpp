#pragma once
#include <functional>                   // function
#include <dctl/successor/enumeration/enumerator_fwd.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::pawn, Moves, Position>
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;

public:
        int operator()(Position const& p) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        return select(active_pawns, not_occupied(p));
                else
                        return 0;
        }

        int select(BitBoard active_pawns, BitBoard not_occupied) const
        {
                return branch(active_pawns, not_occupied);
        }

private:
        int branch(BitBoard active_pawns, BitBoard not_occupied) const
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns, not_occupied) +
                        parallelize<typename Compass::right_up>(active_pawns, not_occupied)
                );
        }

        template<typename Direction>
        int parallelize(BitBoard active_pawns, BitBoard not_occupied) const
        {
                return bit::count(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
