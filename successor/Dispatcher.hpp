#pragma once
#include <boost/utility.hpp>    // boost::noncopyable
#include "State.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"

namespace dctl {
namespace successor {

template<typename> class StateInterface;

/*

        The Dispatcher class forms the <FlyWeightFactory> in a <FlyWeight>
        design Pattern, with the StateInterface class as the <FlyWeight>, 
        and the State class as the <ConcreteFlyWeight>. Examples of 
        the <Client> include the Successor and Mobility classes.

*/

template
<
        typename Selection, 
        typename Rules, 
        typename Board
> 
class Dispatcher
: 
        private boost::noncopyable      // enforce singleton semantics
{
public:
        // typedefs
        typedef const StateInterface<Board>* StatePointer;
        static StatePointer select(int state)
        {
                return instance()[state];
        }

private:
        // implementation
        typedef const StatePointer StateArray[8];
        static StateArray& instance()
        {
                static const State<Side::black, Material::none, Selection, Rules, Board> black_none;
                static const State<Side::black, Material::pawn, Selection, Rules, Board> black_pawn;
                static const State<Side::black, Material::king, Selection, Rules, Board> black_king;
                static const State<Side::black, Material::both, Selection, Rules, Board> black_both;
                static const State<Side::white, Material::none, Selection, Rules, Board> white_none;
                static const State<Side::white, Material::pawn, Selection, Rules, Board> white_pawn;
                static const State<Side::white, Material::king, Selection, Rules, Board> white_king;
                static const State<Side::white, Material::both, Selection, Rules, Board> white_both;

                // Meyers Singleton, see Modern C++ Design p.117
                static const StateArray singleton_ = {
                        &black_none, &black_pawn, &black_king, &black_both, 
                        &white_none, &white_pawn, &white_king, &white_both
                };
                return singleton_;
        }
};

}       // namespace successor
}       // namespace dctl
