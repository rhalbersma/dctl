#pragma once
#include "../node/Stack.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

template<typename> class Position;

namespace successor {

/*

The Driver class forms the <ConcreteClass> in a <Template Method Pattern>,
with the State class as the <AbstractClass>.

*/

template<bool Color, int Material, typename Selection, typename Rules, typename Board> 
class Driver
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&) {}
        static int count(const Position<Board>&) { return 0; }
        static bool detect(const Position<Board>&) { return false; }
};

}       // namespace successor
}       // namespace dctl
