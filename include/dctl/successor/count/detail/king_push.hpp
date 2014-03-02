#pragma once
#include <dctl/successor/count/detail/primary_fwd.hpp>
#include <dctl/successor/propagate/push.hpp>
#include <dctl/successor/select/push.hpp>
#include <dctl/pieces/king.hpp>

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/wave/patterns.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves enumeration
template<bool Color, class Position>
struct Count<Color, pieces::king, select::push, Position>
{
        // enforce reference semantics
        Count(Count const&) = delete;
        Count& operator=(Count const&) = delete;

private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::push, Position>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit Count(State const& p)
        :
                propagate_{p}
        {}

        // function call operators

        template<class Set>
        int operator()(Set const& active_kings) const
        {
                return active_kings.empty() ? 0 : branch(active_kings);
        }

private:
        template<class Set>
        int branch(Set const& active_kings) const
        {
                return
                        parallelize<Compass::left_down >(active_kings) +
                        parallelize<Compass::right_down>(active_kings) +
                        parallelize<Compass::left_up   >(active_kings) +
                        parallelize<Compass::right_up  >(active_kings)
                ;
        }

        template<int Direction, class Set>
        int parallelize(Set const& active_kings) const
        {
                return Sink<Board, Direction, typename rules::range::move<Rules>::type>()(
                        active_kings, propagate_.path()
                ).size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
