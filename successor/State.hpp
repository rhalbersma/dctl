#pragma once
#include "StateInterface.hpp"
#include "Selection.hpp"
#include "Driver.hpp"                   // primary template
#include "Legal.hpp"                    // partially specialized on selection
#include "Conversion.hpp"               // partially specialized on selection
#include "BothJumps.hpp"                // partially specialized on material and selection
#include "BothMoves.hpp"                // partially specialized on material and selection
#include "BothReverse.hpp"              // partially specialized on material and selection
#include "KingJumps.hpp"                // partially specialized on material and selection
#include "KingMoves.hpp"                // partially specialized on material and selection
#include "KingReverse.hpp"              // partially specialized on material and selection
#include "PawnJumps.hpp"                // partially specialized on material and selection
#include "PawnMoves.hpp"                // partially specialized on material and selection
#include "PawnReverse.hpp"              // partially specialized on material and selection
#include "PawnPromotion.hpp"            // partially specialized on material and selection
#include "../node/Stack.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

/*

        The State class forms the <ConcreteState> in a <State> 
        Design Pattern, with the StateInterface class as the <State>. 
        Examples of the <Context> include the Successor and Mobility classes.

        The State class also forms the <ConcreteFlyWeight> in a <FlyWeight>
        Design Pattern, with the StateInterface class as the <FlyWeight>, 
        and the Dispatcher class as the <FlyWeightFactory>. Examples of 
        the <Client> include the Successor and Mobility classes.

*/

template
<
        bool Color, 
        int Material, 
        typename Selection, 
        typename Rules, 
        typename Board
> 
class State
: 
        public StateInterface<Board>
{
private:		
        virtual void do_generate(const Position<Board>& p, Stack& moves) const 
        { 
                Driver<Color, Material, Selection, Rules, Board>::generate(p, moves);
        }

        virtual int do_count(const Position<Board>& p) const
        {
                return Driver<Color, Material, Selection, Rules, Board>::count(p);
        }

        virtual bool do_detect(const Position<Board>& p) const 
        { 
                return Driver<Color, Material, Selection, Rules, Board>::detect(p);
        }
};

}       // namespace successor
}       // namespace dctl
