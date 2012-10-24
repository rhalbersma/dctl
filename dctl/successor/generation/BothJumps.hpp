#pragma once
#include <functional>                   // function
#include <type_traits>                  // is_same
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/mpl/logical.hpp>        // and_
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/Generator_fwd.hpp>
#include <dctl/successor/generation/KingJumps.hpp>
#include <dctl/successor/generation/PawnJumps.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/capture/State.hpp>
#include <dctl/node/Material.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Jumps, Position> KingJumps;
        typedef generator<Color, Material::pawn, Jumps, Position> PawnJumps;
        typedef typename Position::rules_type Rules;
        typedef capture::State<Position> State;

        // representation

        State& capture_;

public:
        // structors

        explicit generator(State& c)
        :
                capture_(c)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                precedence(p);
        }

private:
        void precedence(Position const& p) const
        {
                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, typename Rules::is_absolute_king_precedence());
        }

        // overload for no absolute king jump precedence
        void precedence_dispatch(Position const& p, boost::mpl::false_) const
        {
                // parentheses around function objects to avoid "C++'s most vexing parse"
                (KingJumps(capture_))(p);
                (PawnJumps(capture_))(p);
        }

        // overload for absolute king jump precedence
        void precedence_dispatch(Position const& p, boost::mpl::true_) const
        {
                // parentheses around function objects to avoid "C++'s most vexing parse"
                (KingJumps(capture_))(p);
                if (capture_.empty())
                        (PawnJumps(capture_))(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
