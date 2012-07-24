#pragma once
#include "Result.hpp"
#include "Dispatcher.hpp"
#include "../node/Stack.hpp"
#include "../node/State.hpp"

namespace dctl {
namespace successor {

template<typename Selection, template<typename, typename> class Position, typename Rules, typename Board>
void generate(Position<Rules, Board> const& p, Stack& moves)
{
        typedef Dispatcher<Selection, generation, Position, Rules, Board> Delegate;
        (*Delegate::select(state(p)))(p, moves);
}

template<typename Selection, template<typename, typename> class Position, typename Rules, typename Board>
int count(Position<Rules, Board> const& p)
{
        typedef Dispatcher<Selection, enumeration, Position, Rules, Board> Delegate;
        return (*Delegate::select(state(p)))(p);
}

template<typename Selection, template<typename, typename> class Position, typename Rules, typename Board>
bool detect(Position<Rules, Board> const& p)
{
        typedef Dispatcher<Selection, detection, Position, Rules, Board> Delegate;
        return (*Delegate::select(state(p)))(p);
}

}       // namespace successor
}       // namespace dctl
