#include <cassert>
#include "Dispatcher.h"
#include "../node/Position.h"

namespace dctl {

template<typename Rules, typename Board, typename Selection>
void Successor<Rules, Board, Selection>::generate(const Position<Board>& p, Stack& moves)
{
        successor::Dispatcher<Rules, Board>::select(state(p))->generate_legal(p, moves);
        assert(invariant(p, moves.size()));
}

template<typename Rules, typename Board, typename Selection>
int Successor<Rules, Board, Selection>::count(const Position<Board>& p)
{
        return successor::Dispatcher<Rules, Board>::select(state(p))->count_legal(p);
}

template<typename Rules, typename Board, typename Selection> template<bool Color>
int Successor<Rules, Board, Selection>::count_mobility(const Position<Board>& p)
{
        return successor::Dispatcher<Rules, Board>::select(state<Color>(p))->count_moves(p);
}

template<typename Rules, typename Board, typename Selection>
bool Successor<Rules, Board, Selection>::detect(const Position<Board>& p)
{
        return successor::Dispatcher<Rules, Board>::select(state(p))->detect_legal(p);
}

template<typename Rules, typename Board, typename Selection>
bool Successor<Rules, Board, Selection>::invariant(const Position<Board>& p, int number)
{
        return (
                (count(p) == number) && 
                (detect(p) == (number > 0))
        );
}

template<typename Rules, typename Board, typename Selection>
int Successor<Rules, Board, Selection>::state(const Position<Board>& p)
{
        return state(p.active_color(), active_kings(p), active_men(p));
}

template<typename Rules, typename Board, typename Selection> template<bool Color>
int Successor<Rules, Board, Selection>::state(const Position<Board>& p)
{
        return state(Color, p.kings(Color), p.men(Color));
}

template<typename Rules, typename Board, typename Selection>
int Successor<Rules, Board, Selection>::state(bool color, BitBoard kings, BitBoard men)
{
        return (
                ((color != 0) << 2) +
                ((kings != 0) << 1) +
                ((  men != 0) << 0)
        );
}

}       // namespace dctl
