#pragma once
#include <boost/utility.hpp>            // noncopyable
#include "StateInterface_fwd.hpp"
#include "State.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"

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
        typename Board
>
struct Dispatcher
:
        private boost::noncopyable      // enforce singleton semantics
{
        // typedefs
        typedef StateInterface<Board> const * BaseConstPointer;

        static BaseConstPointer select(int state /* partial intrinsic state encoding */ )
        {
                // TODO: rewrite using C++11 template aliases
                /*

                template<bool Color, int Material> 
                using Derived = State<Color, Material, Selection, Rules, Board>;

                static const Derived<Side::black, Material::none> black_none;
                ...
                static const Derived<Side::black, Material::none> white_both;

                */

                // full intrinsic state representations
                static const State<Side::black, Material::none, Selection, Rules, Board> black_none;
                static const State<Side::black, Material::pawn, Selection, Rules, Board> black_pawn;
                static const State<Side::black, Material::king, Selection, Rules, Board> black_king;
                static const State<Side::black, Material::both, Selection, Rules, Board> black_both;
                static const State<Side::white, Material::none, Selection, Rules, Board> white_none;
                static const State<Side::white, Material::pawn, Selection, Rules, Board> white_pawn;
                static const State<Side::white, Material::king, Selection, Rules, Board> white_king;
                static const State<Side::white, Material::both, Selection, Rules, Board> white_both;

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
