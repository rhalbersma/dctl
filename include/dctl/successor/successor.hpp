#pragma once
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/successor/generate/specializations.hpp>  // Generate
#include <dctl/successor/count/specializations.hpp>     // Count
#include <dctl/successor/detect/specializations.hpp>    // Detect
#include <dctl/successor/select/legal.hpp>              // legal

namespace dctl {
namespace successor {

template<class Select = select::legal, bool RemoveDuplicateJumps = true, bool Reverse = false>
class Successor
{
        template<Color ToMove, class Position>
        auto invariant(Position const& p, std::size_t n)
        {
                return
                        (count <ToMove>(p) ==  n     ) &&
                        (detect<ToMove>(p) == (n > 0))
                ;
        }

public:
        template<Color ToMove, class Position, class Sequence>
        auto generate(Position const& p, Sequence& moves)
        {
                Generate<ToMove, Select, RemoveDuplicateJumps, Reverse>{}(p, moves);
                assert((invariant<ToMove>(p, moves.size())));
        }

        template<class Position, class Sequence>
        auto generate(Position const& p, Sequence& moves)
        {
                return
                        (p.to_move() == Color::black) ?
                        generate<Color::black>(p, moves) :
                        generate<Color::white>(p, moves)
                ;
        }

        template<Color ToMove, class Position>
        auto count(Position const& p)
        {
                return Count<ToMove, Select, RemoveDuplicateJumps, Reverse>{}(p);
        }

        template<class Position>
        auto count(Position const& p)
        {
                return
                        (p.to_move() == Color::black) ?
                        count<Color::black>(p) :
                        count<Color::white>(p)
                ;
        }

        template<Color ToMove, class Position>
        auto detect(Position const& p)
        {
                return Detect<ToMove, Select, Reverse>{}(p);
        }

        template<class Position>
        auto detect(Position const& p)
        {
                return
                        (p.to_move() == Color::black) ?
                        detect<Color::black>(p) :
                        detect<Color::white>(p)
                ;
        }
};

}
}       // namespace dctl
