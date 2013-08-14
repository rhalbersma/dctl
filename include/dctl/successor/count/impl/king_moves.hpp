#pragma once
#include <boost/utility.hpp>
#include <dctl/successor/count/impl/primary_fwd.hpp>
#include <dctl/successor/propagate/moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/pieces/king.hpp>

#include <dctl/bit/bit.hpp>
#include <dctl/angle/compass.hpp>
#include <dctl/board/patterns.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king moves enumeration
template<bool Color, class Position>
struct count<Color, pieces::king, select::moves, Position>
:
        // enforce reference semantics
        boost::noncopyable
{
private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Compass = angle::Compass<Color, Board>;
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

        int operator()(BitBoard active_kings) const
        {
                return active_kings? branch(active_kings) : 0;
        }

private:
        int branch(BitBoard active_kings) const
        {
                return (
                        parallelize< Compass::left_down  >(active_kings) +
                        parallelize< Compass::right_down >(active_kings) +
                        parallelize< Compass::left_up    >(active_kings) +
                        parallelize< Compass::right_up   >(active_kings)
                );
        }

        template<int Direction>
        int parallelize(BitBoard active_kings) const
        {
                return bit::size(
                        Sink<Board, Direction, typename rules::range::move<Rules>::type>()(active_kings, propagate_.path())
                );
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
