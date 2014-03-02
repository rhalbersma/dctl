#pragma once
#include <dctl/successor/count/impl/primary_fwd.hpp>
#include <dctl/successor/propagate/moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/pieces/pawn.hpp>

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/wave/patterns.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for pawn moves enumeration
template<bool Color, class Position>
struct count<Color, pieces::pawn, select::moves, Position>
{
        // enforce reference semantics
        count(count const&) = delete;
        count& operator=(count const&) = delete;

private:
        using Board = typename Position::board_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::moves, Position>;

        // representation

        State const& propagate_;

public:
        // structors

        explicit count(State const& p)
        :
                propagate_{p}
        {}

        // function call operators

        template<class Set>
        int operator()(Set const& active_pawns) const
        {
                return active_pawns.empty() ? 0 : branch(active_pawns);
        }

private:
        template<class Set>
        int branch(Set const& active_pawns) const
        {
                return
                        parallelize<Compass::left_up >(active_pawns) +
                        parallelize<Compass::right_up>(active_pawns)
                ;
        }

        template<int Direction, class Set>
        int parallelize(Set const& active_pawns) const
        {
                return Sink<Board, Direction, rules::range::distance_1>()(
                        active_pawns, propagate_.path()
                ).size();
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
