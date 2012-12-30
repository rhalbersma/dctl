#pragma once
#include <type_traits>                  // false_type, true_type
#include <boost/mpl/logical.hpp>        // and_
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/successor/generation/king_jumps.hpp>
#include <dctl/successor/generation/pawn_jumps.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/capture/state.hpp>
#include <dctl/node/material.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable
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

        /*explicit*/ generator(State& c)
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
                precedence_dispatch(p, typename rules::traits<Rules>::is_absolute_king_precedence());
        }

        // overload for no absolute king jump precedence
        void precedence_dispatch(Position const& p, std::false_type) const
        {
                KingJumps{capture_}(p);
                PawnJumps{capture_}(p);
        }

        // overload for absolute king jump precedence
        void precedence_dispatch(Position const& p, std::true_type) const
        {
                KingJumps{capture_}(p);
                if (capture_.empty())
                        PawnJumps{capture_}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
