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
private:
        /*

        TODO: use C++11 template aliases

        template<bool Color, int Material> 
        using Delegate = 
                State<Color, Material, Selection, Rules, Board, Position>
        ;

        */

        template<bool Color, int Material>
        struct Delegate
        :
                State<Color, Material, Selection, Rules, Board, Position>
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
                        &black_none, &black_pawn, &black_king, &black_both,
                        &white_none, &white_pawn, &white_king, &white_both
                };

                return singleton_[state];
        }
};

}       // namespace successor
}       // namespace dctl
