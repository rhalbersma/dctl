#pragma once
#include <dctl/angle/traits.hpp>        // is_positive
#include <dctl/board/Shift.hpp>         // shift_size, Shift 

namespace dctl {
namespace detail {
namespace loop {

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
template<typename Sign, typename N, typename T>
T fill(T generator, T propagator)
{
        T flood(0);
        while (generator) {
                flood |= generator;
                generator = Shift<Sign, N>()(generator) & propagator;
        }
        return (flood);
}

}       // namespace loop

// direction-wise flood-fill generator over propagator
template<typename Sign, typename N, typename T>
T fill(T generator, T propagator)
{
        return (loop::fill<Sign, N>(generator, propagator));
}

}       // namespace detail 

template<typename Board, typename Direction>
struct Fill
{
        template<typename T>
        T operator()(T generator, T propagator) const
        {
                return (detail::fill< typename
                        angle::lazy::is_positive<Direction>::type, typename 
                        shift_size<Board, Direction>::type
                >(generator, propagator));
        }
};

}       // namespace dctl
