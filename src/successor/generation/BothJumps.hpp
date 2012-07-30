#pragma once
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "Generator_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "../Select.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, select::Jumps, typename Position>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef generator<Color, Material::king, select::Jumps, Position> KingJumps;
        typedef generator<Color, Material::pawn, select::Jumps, Position> PawnJumps;
        typedef typename Position::rules_type Rules;
        typedef capture::State<Position> State;

public:
        static void run(Position const& p, Stack& moves)
        {
                capture::State<Position> capture(p, moves); 
                run(p, capture);
        }

private:
        static void run(Position const& p, State& capture)
        {
                // tag dispatching on absolute king capture precedence
                run_dispatch(p, capture, typename Rules::is_absolute_king_precedence());
        }

        // overload for no absolute king capture precedence
        static void run_dispatch(Position const& p, State& capture, boost::mpl::false_)
        {
                KingJumps::run(p, capture);
                PawnJumps::run(p, capture);
        }

        // overload for absolute king capture precedence
        static void run_dispatch(Position const& p, State& capture, boost::mpl::true_)
        {
                KingJumps::run(p, capture);
                if (capture.empty()) {
                        PawnJumps::run(p, capture);
                }
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
