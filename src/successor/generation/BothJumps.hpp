#pragma once
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "../Driver_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Jumps, generation, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::king, select::Jumps, generation, Rules, Board> KingJumps;
        typedef Driver<Color, Material::pawn, select::Jumps, generation, Rules, Board> PawnJumps;
        typedef capture::State<Rules, Board> State;

public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                State capture(p, moves);
                generate(p, capture);
        }

private:
        template<typename Position>
        static void generate(Position const& p, State& capture)
        {
                // tag dispatching on absolute king capture precedence
                generate_dispatch(p, capture, typename Rules::is_absolute_king_precedence());
        }

        // overload for no absolute king capture precedence
        template<typename Position>
        static void generate_dispatch(Position const& p, State& capture, boost::mpl::false_)
        {
                KingJumps::generate(p, capture);
                PawnJumps::generate(p, capture);
        }

        // overload for absolute king capture precedence
        template<typename Position>
        static void generate_dispatch(Position const& p, State& capture, boost::mpl::true_)
        {
                KingJumps::generate(p, capture);
                if (capture.empty())
                        PawnJumps::generate(p, capture);
        }
};

}       // namespace successor
}       // namespace dctl
