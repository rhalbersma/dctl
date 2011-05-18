#pragma once
#include "../Move/Stack.h"
#include "../Move/Types.h"
#include "../Node/Pieces.h"
#include <cassert>
#include <cstddef>

namespace Tree {

namespace Node { template<typename> class Position; }
namespace Move { class Stack; }

namespace Generate {

template<bool, int, Move::Type, typename Rules, typename Board>
class Template
{
public:
        static void generate(const Node::Position<Board>&, Move::Stack&) { assert(false); };
        static size_t count(const Node::Position<Board>&) { assert(false); return 0; };
        static bool detect(const Node::Position<Board>&) { assert(false); return false; };
};

}       // namespace Generate
}       // namespace Tree
