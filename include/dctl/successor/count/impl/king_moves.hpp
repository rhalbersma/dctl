#pragma once
#include <boost/utility.hpp>
#include <dctl/successor/count/impl/primary_fwd.hpp>
#include <dctl/successor/material/king.hpp>
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

// partial specialization for king moves enumeration
template<bool Color, typename Position>
struct count<Color, material::king, select::moves, Position>
:
        // enforce reference semantics
        boost::noncopyable
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
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

        int operator()(BitBoard active_kings) const
        {
                return active_kings? branch(active_kings) : 0;
        }

private:
        int branch(BitBoard active_kings) const
        {
                return (
                        parallelize<typename Compass::left_down >(active_kings) +
                        parallelize<typename Compass::right_down>(active_kings) +
                        parallelize<typename Compass::left_up   >(active_kings) +
                        parallelize<typename Compass::right_up  >(active_kings)
                );
        }

        template<typename Direction>
        int parallelize(BitBoard active_kings) const
        {
                return bit::count(
                        Sink<Board, Direction, typename rules::range_king<Rules>::type>()(active_kings, propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
