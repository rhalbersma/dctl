#pragma once
#include <boost/utility.hpp>
#include <dctl/successor/count/impl/primary_fwd.hpp>
#include <dctl/successor/propagate/moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/pieces/pawn.hpp>

#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for pawn moves enumeration
template<bool Color, class Position>
struct count<Color, pieces::pawn, select::moves, Position>
:
        // enforce reference semantics
        boost::noncopyable
{
private:
        using Board = typename Position::board_type;
        using Compass = board::Compass<Color, Board>;
        using State = Propagate<select::moves, Position>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit count(State const& p)
        :
                propagate_(p)
        {}

        // function call operators

        int operator()(BitBoard active_pawns) const
        {
                return active_pawns? branch(active_pawns) : 0;
        }

private:
        int branch(BitBoard active_pawns) const
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns) +
                        parallelize<typename Compass::right_up>(active_pawns)
                );
        }

        template<class Direction>
        int parallelize(BitBoard active_pawns) const
        {
                return bit::size(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
