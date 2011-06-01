#pragma once
#include <cstddef>
#include "../Move/Stack.h"
#include "../Move/Types.h"

namespace Tree {

namespace Node { template<typename> class Position; }
namespace Move { class Stack; }

namespace Generate {

template<bool, int NodePieces, Move::Type, typename Rules, typename Board>
class Template
{
public:
        static void generate(const Node::Position<Board>&, Move::Stack&);
        static size_t count(const Node::Position<Board>&);
        static bool detect(const Node::Position<Board>&);
};

}       // namespace Generate
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Template.hpp"
