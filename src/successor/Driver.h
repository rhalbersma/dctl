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

template<bool, int, int, typename Rules, typename Board>
class Driver
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate_regular(const Position<Board>&, Stack&);
        static void generate_reverse(const Position<Board>&, Stack&);
        static void generate_promotions(const Position<Board>&, Stack&);

        static int count_regular(const Position<Board>&);
        static int count_reverse(const Position<Board>&);
        static int count_promotions(const Position<Board>&);
        
        static bool detect_regular(const Position<Board>&);
        static bool detect_reverse(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "Driver.hpp"
