#pragma once
#include "../node/Stack.h"

namespace dctl {

template<typename> class Position;

namespace successors {

template<bool, int, int, typename Rules, typename Board>
class Driver
{
public:
        static void generate(const Position<Board>&, Stack&);
        static void generate_reverse(const Position<Board>&, Stack&);
        static void generate_promotions(const Position<Board>&, Stack&);

        static int count(const Position<Board>&);
        static int count_reverse(const Position<Board>&);
        static int count_promotions(const Position<Board>&);
        
        static bool detect(const Position<Board>&);
        static bool detect_reverse(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);
};

}       // namespace successors
}       // namespace dctl

// include template definitions inside header
#include "Driver.hpp"
