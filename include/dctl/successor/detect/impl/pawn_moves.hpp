#pragma once
#include <dctl/successor/detect/impl/primary_fwd.hpp>
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>

#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for pawn moves detection
template<bool Color, typename Position, typename Range>
struct detect<Color, material::pawn, select::moves, Position, Range>
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;
        typedef Propagate<select::moves, Position> State;

        // representation

        State const& propagate_;

public:
        // structors

        explicit detect(State const& p)
        :
                propagate_(p)
        {}

        // function call operators

        bool operator()(BitBoard active_pawns) const
        {
                return active_pawns? branch(active_pawns) : false;
        }

private:
        bool branch(BitBoard active_pawns) const
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns) ||
                        parallelize<typename Compass::right_up>(active_pawns)
                );
        }

        template<typename Direction>
        bool parallelize(BitBoard active_pawns) const
        {
                return !bit::empty(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
