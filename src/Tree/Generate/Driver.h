#pragma once
#include <cstddef>
#include "../Node/Stack.h"

namespace tree {

namespace node { template<typename> class Position; }

namespace generate {

template<bool, int NodePieces, node::Type, typename Rules, typename Board>
class Driver
{
public:
        static void generate(const node::Position<Board>&, node::Stack&);
        static void generate_reverse(const node::Position<Board>&, node::Stack&);
        static void generate_promotions(const node::Position<Board>&, node::Stack&);

        static size_t count(const node::Position<Board>&);
        static size_t count_reverse(const node::Position<Board>&);
        static size_t count_promotions(const node::Position<Board>&);
        
        static bool detect(const node::Position<Board>&);
        static bool detect_reverse(const node::Position<Board>&);
        static bool detect_promotions(const node::Position<Board>&);
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Driver.hpp"
