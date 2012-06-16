#pragma once
#include "Dispatcher.hpp"
#include "../node/Stack.hpp"
#include "../node/State.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {

/*

        The Successor class forms a <Context> in a <State> Design Pattern,
        with the StateInterface class as the <State> and the State class
        as the <ConcreteState>.

        FlyWeight Design Pattern
        ------------------------
        FlyWeight               : StateInterface
        ConcreteFlyWeight       : State
        FlyWeightFactory        : Dispatcher
        Client                  : Successor, Mobility

        A particular position's state is determined by its side to move
        and the availability of kings and pawns among its pieces. For efficiency,
        all 8 unique position states are shared through the <FlyWeightFactory>.

*/

template
<
        typename Selection
>
struct Successor
:
        // enforce static semantics
        private nonconstructible
{
public:
        template<template<typename, typename> class Position, typename Rules, typename Board>
        static void generate(Position<Rules, Board> const& p, Stack& moves)
        {
                successor::Dispatcher<Selection, Position, Rules, Board>::select(state(p))->generate(p, moves);
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        static int count(Position<Rules, Board> const& p)
        {
                return successor::Dispatcher<Selection, Position, Rules, Board>::select(state(p))->count(p);
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        static bool detect(Position<Rules, Board> const& p)
        {
                return successor::Dispatcher<Selection, Position, Rules, Board>::select(state(p))->detect(p);
        }
};

}       // namespace dctl
