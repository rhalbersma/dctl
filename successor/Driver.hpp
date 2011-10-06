#pragma once
#include "../node/Stack.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

/*

        The Driver class forms the <ConcreteClass> in a <Template Method>
        design pattern, with the State class as the <AbstractClass>.

*/

template
<
        bool Color, 
        int Move, 
        typename Selection, 
        typename Rules, 
        typename Board
> 
class Driver
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
