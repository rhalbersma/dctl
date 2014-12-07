#pragma once
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/successor/generate/specializations.hpp>  // Generate
#include <dctl/successor/invariant.hpp>                 // invariant
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/move/move.hpp>                           // Move
#include <dctl/type_traits.hpp>                         // value_type_t
#include <dctl/utility/stack_vector.hpp>                // DCTL_PP_STACK_RESERVE
#include <memory>                                       // allocator, allocator_traits
#include <vector>                                       // vector

namespace dctl {
namespace successor {

template
<
        Color ToMove, bool IsReverse = false, class Select = select::legal, class... Args,
        class Position, class Allocator = std::allocator<Move_t<Position>>
>
auto generate(Position const& p, Allocator const& alloc = Allocator())
{
        using Move = value_type_t<std::allocator_traits<Allocator>>;
        std::vector<Move, Allocator> moves(alloc);
        moves.reserve(DCTL_PP_STACK_RESERVE);
        Generate<ToMove, IsReverse, Select, Args...>{}(p, moves);
        assert((invariant<ToMove, IsReverse, Select, Args...>(p, moves.size())));
        return moves;
}

template
<
        bool IsReverse = false, class Select = select::legal, class... Args,
        class Position, class Allocator = std::allocator<Move_t<Position>>
>
auto generate(Position const& p, Allocator const& alloc = Allocator())
{
        return
                (p.to_move() == Color::black) ?
                generate<Color::black, IsReverse, Select, Args...>(p, alloc) :
                generate<Color::white, IsReverse, Select, Args...>(p, alloc)
        ;
}

}       // namespace successor
}       // namespace dctl
