#include <cassert>
#include "Dispatcher.h"
#include "../node/Position.h"

namespace dctl {

template<typename Selection, typename Rules> template<typename Board>
void Successor<Selection, Rules>::generate(const Position<Board>& p, Stack& moves)
{
        successor::Dispatcher<Rules, Board>::select(state(p))->generate_legal(p, moves);
        assert(invariant(p, moves.size()));
}

template<typename Selection, typename Rules> template<typename Board>
int Successor<Selection, Rules>::count(const Position<Board>& p)
{
        return successor::Dispatcher<Rules, Board>::select(state(p))->count_legal(p);
}

template<typename Selection, typename Rules> template<bool Color, typename Board>
int Successor<Selection, Rules>::mobility(const Position<Board>& p)
{
        return successor::Dispatcher<Rules, Board>::select(state<Color>(p))->count_moves(p);
}

template<typename Selection, typename Rules> template<typename Board>
bool Successor<Selection, Rules>::detect(const Position<Board>& p)
{
        return successor::Dispatcher<Rules, Board>::select(state(p))->detect_legal(p);
}

template<typename Selection, typename Rules> template<typename Board>
bool Successor<Selection, Rules>::invariant(const Position<Board>& p, int number)
{
        return (
                (count(p) == number) && 
                (detect(p) == (number > 0))
        );
}

}       // namespace dctl
