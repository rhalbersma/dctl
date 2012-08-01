#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include <boost/mpl/logical.hpp>        // and_
#include "Generator_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "../Select.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Jumps, typename Position>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Jumps, Position> KingJumps;
        typedef generator<Color, Material::pawn, Jumps, Position> PawnJumps;
        typedef typename Position::rules_type Rules;
        typedef capture::State<Position> State;

public:
        void operator()(Position const& p, Stack& moves)
        {
                capture::State<Position> capture(p, moves); 
                precedence(p, capture);
        }

private:
        void precedence(Position const& p, State& capture)
        {
                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, capture, typename Rules::is_absolute_king_precedence());
        }

        // overload for no absolute king jump precedence
        void precedence_dispatch(Position const& p, State& capture, boost::mpl::false_)
        {
                KingJumps()(p, capture);
                PawnJumps()(p, capture);
        }

        // overload for absolute king jump precedence
        void precedence_dispatch(Position const& p, State& capture, boost::mpl::true_)
        {
                KingJumps()(p, capture);
                if (capture.empty())
                        PawnJumps()(p, capture);                
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
