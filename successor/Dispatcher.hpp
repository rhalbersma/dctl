#pragma once
#include "StateInterface_fwd.hpp"
#include "State.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

/*

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface
        ConcreteFlyWeight       : State
        FlyWeightFactory        : Dispatcher
        Client                  : Successor, Mobility

*/

template
<
        typename Selection,
        typename Rules,
        typename Board,
        template<typename, typename> class Position
>
struct Dispatcher
:
        // enforce static semantics
        private nonconstructible
{
        // typedefs

        typedef StateInterface< Position<Rules, Board> > const * BaseConstPointer;

        static BaseConstPointer select(int state /* partial intrinsic state encoding */ )
        {
                /*

                TODO: suppress outside template parameters using C++11 template aliases

                template<bool Color, int Material> 
                using Delegate = State<Color, Material, Selection, Rules, Board, Position>;

                static const Delegate<Side::black, Material::none> black_none;
                ...
                static const Delegate<Side::black, Material::none> white_both;

                */

                // full intrinsic state representations
                static const State<Side::black, Material::none, Selection, Rules, Board, Position> black_none;
                static const State<Side::black, Material::pawn, Selection, Rules, Board, Position> black_pawn;
                static const State<Side::black, Material::king, Selection, Rules, Board, Position> black_king;
                static const State<Side::black, Material::both, Selection, Rules, Board, Position> black_both;
                static const State<Side::white, Material::none, Selection, Rules, Board, Position> white_none;
                static const State<Side::white, Material::pawn, Selection, Rules, Board, Position> white_pawn;
                static const State<Side::white, Material::king, Selection, Rules, Board, Position> white_king;
                static const State<Side::white, Material::both, Selection, Rules, Board, Position> white_both;

                // "Meyers Singleton", Effective C++ 3rd ed., Item 4 (p. 31-32)
                static BaseConstPointer const singleton_[] = {
                        &black_none, &black_pawn, &black_king, &black_both,
                        &white_none, &white_pawn, &white_king, &white_both
                };

                return singleton_[state];
        }
};

}       // namespace successor
}       // namespace dctl
