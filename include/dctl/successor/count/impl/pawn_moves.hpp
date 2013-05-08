#pragma once
#include <cstddef>
#include <boost/utility.hpp>
#include <dctl/successor/count/impl/primary_fwd.hpp>
#include <dctl/successor/material/pawn.hpp>
#include <dctl/successor/propagate/moves.hpp>
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

// partial specialization for pawn moves enumeration
template<bool Color, typename Position>
struct count<Color, material::pawn, select::moves, Position>
:
        // enforce reference semantics
        boost::noncopyable
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

        explicit count(State const& p)
        :
                propagate_(p)
        {}

        // function call operators

        std::size_t operator()(BitBoard active_pawns) const
        {
                return active_pawns? branch(active_pawns) : 0;
        }

private:
        std::size_t branch(BitBoard active_pawns) const
        {
                return (
                        parallelize<typename Compass::left_up >(active_pawns) +
                        parallelize<typename Compass::right_up>(active_pawns)
                );
        }

        template<typename Direction>
        std::size_t parallelize(BitBoard active_pawns) const
        {
                return bit::count(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
