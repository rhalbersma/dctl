#pragma once
#include <boost/utility.hpp>    // boost::noncopyable
#include "State.hpp"
#include "../node/Material.h"
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
                static const State<Side::black, Material::NONE, Selection, Rules, Board> BLACK_NONE_;
                static const State<Side::black, Material::PAWN, Selection, Rules, Board> BLACK_PAWN_;
                static const State<Side::black, Material::KING, Selection, Rules, Board> BLACK_KING_;
                static const State<Side::black, Material::BOTH, Selection, Rules, Board> BLACK_BOTH_;
                static const State<Side::white, Material::NONE, Selection, Rules, Board> WHITE_NONE_;
                static const State<Side::white, Material::PAWN, Selection, Rules, Board> WHITE_PAWN_;
                static const State<Side::white, Material::KING, Selection, Rules, Board> WHITE_KING_;
                static const State<Side::white, Material::BOTH, Selection, Rules, Board> WHITE_BOTH_;

                // Meyers Singleton, see Modern C++ Design p.117
                static const StateArray singleton_ = {
                        &BLACK_NONE_, &BLACK_PAWN_, &BLACK_KING_, &BLACK_BOTH_, 
                        &WHITE_NONE_, &WHITE_PAWN_, &WHITE_KING_, &WHITE_BOTH_
                };
                return singleton_;
        }
};

}       // namespace successor
}       // namespace dctl
