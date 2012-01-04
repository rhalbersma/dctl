#pragma once
#include "Selection.hpp"
#include "StateInterface.hpp"
#include "Driver.hpp"
#include "BothJumps.hpp"
#include "BothMoves.hpp"
#include "KingJumps.hpp"
#include "KingMoves.hpp"
#include "PawnJumps.hpp"
#include "PawnMoves.hpp"
#include "KingReverse.hpp"
#include "PawnReverse.hpp"
#include "Promotion.hpp"
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

        The State class also forms the <AbstractClass> in a <Template Method>
        Design Pattern, with the Driver class as the <ConcreteClass>.

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

template<bool Color, int Material, typename Rules, typename Board> 
class State<Color, Material, Legal, Rules, Board>
: 
        public StateInterface<Board>
{
private:		
        virtual void do_generate(const Position<Board>& p, Stack& moves) const 
        {
                Driver<Color, Material, Jumps, Rules, Board>::generate(p, moves);
                if (moves.empty())
                        Driver<Color, Material, Moves, Rules, Board>::generate(p, moves);
        }

        virtual int do_count(const Position<Board>& p) const
        {
                auto num_moves = Driver<Color, Material, Jumps, Rules, Board>::count(p);
                if (!num_moves)
                        num_moves += Driver<Color, Material, Moves, Rules, Board>::count(p);
                return num_moves;
        }

        virtual bool do_detect(const Position<Board>& p) const 
        { 
                return (
                        Driver<Color, Material, Moves, Rules, Board>::detect(p) || 
                        Driver<Color, Material, Jumps, Rules, Board>::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
