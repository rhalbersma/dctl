#pragma once
#include "../Position/Move.h"
#include "../Position/Pieces.h"
#include <cassert>
#include <cstddef>

template<typename> class Position;
namespace Move { class List; }

template<bool, int, Move::Type, typename Rules, typename Board>
class Template
{
public:
        static void generate(const Position<Board>&, Move::List&) { assert(false); };
        static size_t count(const Position<Board>&) { assert(false); return 0; };
        static bool detect(const Position<Board>&) { assert(false); return false; };
};
