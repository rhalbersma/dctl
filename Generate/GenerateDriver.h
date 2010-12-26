#pragma once
#include "../Position/Move.h"
#include "../Position/Reversible/Pieces.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
template<typename, typename> class Propagate;

template<bool, Pieces::Composition, Move::Type, typename Rules, typename Board>
class GenerateDriver
{
public:
        static void generate(const Position<Board>&, Propagate<Rules, Board>&) {};
        static void generate_captures(const Position<Board>&, Propagate<Rules, Board>&) {};
        static void generate_promotions(const Position<Board>&, Propagate<Rules, Board>&) {};

        static size_t count(const Position<Board>&) { return 0; };
        static size_t count_moves(const Position<Board>&) { return 0; };

        static bool detect(const Position<Board>&) { return false; };
        static bool detect_captures(const Position<Board>&) { return false; };
        static bool detect_promotions(const Position<Board>&) { return false; };
};
