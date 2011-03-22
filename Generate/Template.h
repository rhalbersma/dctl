#pragma once
#include "../Move/Stack.h"
#include "../Position/Pieces.h"
#include <cassert>
#include <cstddef>

template<typename> class Position;
namespace Move { class Stack; }

template<bool, int, Move::Type, typename Rules, typename Board>
class Template
{
public:
        static void generate(const Position<Board>&, Move::Stack&) { assert(false); };
        static size_t count(const Position<Board>&) { assert(false); return 0; };
        static bool detect(const Position<Board>&) { assert(false); return false; };
};
