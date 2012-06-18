#pragma once
#include <array>                        // array
#include "StateInterface_fwd.hpp"
#include "State.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

/*

        For purposes of move generation, a position has 8 possible states,
        depending on the side to move and the availability of kings and pawns. 
        The move generator *dynamically* dispatches the state-specific routines.
        For efficiency, the states are *statically* stored in a FlyWeightFactory.

        State Design Pattern
        --------------------
        State                   : StateInterface
        ConcreteState           : State
        Context                 : Successor, Mobility

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface
        ConcreteFlyWeight       : State
        FlyWeightFactory        : Dispatcher <---------------------- this class
        Client                  : Successor, Mobility

*/

template
<
        typename Selection,
        template<typename, typename> class Position,
        typename Rules,
        typename Board
>
struct Dispatcher
:
        // enforce static semantics
        private nonconstructible
{
private:
        /*

        TODO: use C++11 template aliases

        template<bool Color, int Material> 
        using Delegate = 
                State<Color, Material, Selection, Position, Rules, Board>
        ;

        */

        template<bool Color, int Material>
        struct Delegate
        :
                State<Color, Material, Selection, Position, Rules, Board>
        {};

public:
        // typedefs

        typedef StateInterface< Position<Rules, Board> > const* BaseConstPointer;

        static BaseConstPointer select(int state /* partial intrinsic state encoding */ )
        {
                // full intrinsic state representations
                static Delegate<Side::black, Material::none> const black_none;
                static Delegate<Side::black, Material::pawn> const black_pawn;
                static Delegate<Side::black, Material::king> const black_king;
                static Delegate<Side::black, Material::both> const black_both;
                static Delegate<Side::white, Material::none> const white_none;
                static Delegate<Side::white, Material::pawn> const white_pawn;
                static Delegate<Side::white, Material::king> const white_king;
                static Delegate<Side::white, Material::both> const white_both;

                // "Meyers Singleton", Effective C++ 3rd ed., Item 4 (p. 31-32)
                static BaseConstPointer const singleton_[] = {
                //      no material  only pawns   only kings   kings and pawns
                        &black_none, &black_pawn, &black_king, &black_both,     // black to move
                        &white_none, &white_pawn, &white_king, &white_both      // white to move
                };

                return singleton_[state];
        }
};

}       // namespace successor
}       // namespace dctl
