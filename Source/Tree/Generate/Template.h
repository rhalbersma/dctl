#pragma once
#include <cstddef>
#include "../Move/Stack.h"
#include "../Move/Types.h"

namespace tree {

namespace node { template<typename> class Position; }
namespace move { class Stack; }

namespace generate {

template<bool, int NodePieces, move::Type, typename Rules, typename Board>
class Template
{
public:
        static void generate(const node::Position<Board>&, move::Stack&);
        static void generate_promotions(const node::Position<Board>&, move::Stack&);

        static size_t count(const node::Position<Board>&);
        static size_t count_promotions(const node::Position<Board>&);
        
        static bool detect(const node::Position<Board>&);
        static bool detect_promotions(const node::Position<Board>&);
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Template.hpp"
