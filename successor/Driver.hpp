#pragma once
#include "../node/Stack.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

template
<
        bool Color, 
        int Material, 
        typename Selection, 
        typename Rules, 
        typename Board
> 
struct Driver
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&) 
        { 
                /* no-op */ 
        }

        static int count(const Position<Board>&) 
        { 
                return 0; 
        }

        static bool detect(const Position<Board>&) 
        { 
                return false; 
        }
};

}       // namespace successor
}       // namespace dctl
