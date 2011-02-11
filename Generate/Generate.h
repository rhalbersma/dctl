#pragma once
#include <cstddef>

template<typename> class Position;
class MoveList;

template<typename Rules, typename Board>
class Generate
{
public:
        static void generate(const Position<Board>&, MoveList&);
        static void generate_captures(const Position<Board>&, MoveList&);
        static void generate_promotions(const Position<Board>&, MoveList&);

        static size_t count(const Position<Board>&);

        static bool detect(const Position<Board>&);
        static bool detect_captures(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

        static bool invariant(const Position<Board>&, size_t);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Generate.hpp"
