#pragma once
#include <boost/utility.hpp>
#include <dctl/successor/detect/impl/primary_fwd.hpp>
#include <dctl/successor/material/king.hpp>             // king
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

// partial specialization for king moves detection
template<bool Color, typename Position, typename Range>
struct detect<Color, material::king, select::moves, Position, Range>
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

        explicit detect(State const& p)
        :
                propagate_(p)
        {}

        // function call operators

        bool operator()(BitBoard active_kings) const
        {
                return active_kings? branch(active_kings) : false;
        }

private:
        bool branch(BitBoard active_kings) const
        {
                return (
                        parallelize<typename Compass::left_down >(active_kings) ||
                        parallelize<typename Compass::right_down>(active_kings) ||
                        parallelize<typename Compass::left_up   >(active_kings) ||
                        parallelize<typename Compass::right_up  >(active_kings)
                );
        }

        template<typename Direction>
        bool parallelize(BitBoard active_kings) const
        {
                return !bit::empty(
                        Sink<Board, Direction, rules::range::distance_1>()(active_kings, propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
