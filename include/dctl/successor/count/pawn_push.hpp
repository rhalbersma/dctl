#pragma once
#include <dctl/successor/count/primary_fwd.hpp>
#include <dctl/successor/select/push.hpp>
#include <dctl/pieces/pawn.hpp>

#include <dctl/angle/directions.hpp>                    // left_up, right_up
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/wave/patterns.hpp>
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t
#include <type_traits>

namespace dctl {
namespace successor {

// partial specialization for pawn moves enumeration
template<Color ToMove, class Position>
class Count<ToMove, pieces::pawn, select::push, Position>
{
public:
        // enforce reference semantics
        Count(Count const&) = delete;
        Count& operator=(Count const&) = delete;

private:
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;

        static constexpr auto orientation = orientation_v<Board, ToMove>;

        // representation

        Set const& propagate;

public:
        // constructors

        explicit Count(Set const& p)
        :
                propagate{p}
        {}

        // function call operators

        auto operator()(Set const& active_pawns) const
        {
                return active_pawns.none() ? 0 : branch(active_pawns);
        }

private:
        auto branch(Set const& active_pawns) const
        {
                return
                        parallelize<left_up (orientation)>(active_pawns) +
                        parallelize<right_up(orientation)>(active_pawns)
                ;
        }

        template<int Direction>
        auto parallelize(Set const& active_pawns) const
        {
                return Sink<Board, Direction, std::false_type>{}(
                        active_pawns, propagate
                ).count();
        }
};

}       // namespace successor
}       // namespace dctl
