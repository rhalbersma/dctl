#pragma once
#include <functional>                   // function
#include <type_traits>                  // is_same
#include <boost/mpl/bool.hpp>           // false_, true_
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
:
        public std::function<void(Position const&, Stack&)>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Jumps, Position> KingJumps;
        typedef generator<Color, Material::pawn, Jumps, Position> PawnJumps;
        typedef typename Position::rules_type Rules;
        typedef capture::State<Position> State;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                capture::State<Position> capture(p, moves);
                precedence(p, capture);
        }

private:
        void precedence(Position const& p, State& capture) const
        {
                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, capture, typename Rules::is_absolute_king_precedence());
        }

        // overload for no absolute king jump precedence
        void precedence_dispatch(Position const& p, State& capture, boost::mpl::false_) const
        {
                // parentheses around function objects to avoid "C++'s most vexing parse"
                (KingJumps(capture))(p);
                (PawnJumps(capture))(p);
        }

        // overload for absolute king jump precedence
        void precedence_dispatch(Position const& p, State& capture, boost::mpl::true_) const
        {
                // parentheses around function objects to avoid "C++'s most vexing parse"
                (KingJumps(capture))(p);
                if (capture.empty())
                        (PawnJumps(capture))(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
