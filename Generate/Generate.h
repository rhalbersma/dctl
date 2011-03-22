#pragma once
#include "State.h"
#include "../Position/Pieces.h"
#include "../Position/Side.h"
#include <cstddef>

template<typename> class Position;
namespace Move { class Stack; }
        
template<typename Rules, typename Board>
class Generate
{
public:
        // interface
        static void generate(const Position<Board>&, Move::Stack&);
        static void generate_captures(const Position<Board>&, Move::Stack&);
        static void generate_promotions(const Position<Board>&, Move::Stack&);

        static size_t count(const Position<Board>&);
        static size_t count_captures(const Position<Board>&);
        static size_t count_promotions(const Position<Board>&);
        template<bool> static size_t count_mobility(const Position<Board>&);

        static bool detect(const Position<Board>&);
        static bool detect_captures(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

private:
        // implementation
        static const AbstractState<Rules, Board>* select(const Position<Board>&);
        template<bool> static const AbstractState<Rules, Board>* select(const Position<Board>&);

        static int state(const Position<Board>&);
        template<bool> static int state(const Position<Board>&);
        static int state(bool, BitBoard, BitBoard);

        static bool invariant(const Position<Board>&, size_t);

        // representation
        static const AbstractState<Rules, Board>* const STATE[];

        static const State<Side::BLACK, Pieces::NONE, Rules, Board> BLACK_NONE;
        static const State<Side::BLACK, Pieces::PAWN, Rules, Board> BLACK_PAWN;
        static const State<Side::BLACK, Pieces::KING, Rules, Board> BLACK_KING;
        static const State<Side::BLACK, Pieces::BOTH, Rules, Board> BLACK_BOTH;
        static const State<Side::WHITE, Pieces::NONE, Rules, Board> WHITE_NONE;
        static const State<Side::WHITE, Pieces::PAWN, Rules, Board> WHITE_PAWN;
        static const State<Side::WHITE, Pieces::KING, Rules, Board> WHITE_KING;
        static const State<Side::WHITE, Pieces::BOTH, Rules, Board> WHITE_BOTH;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Generate.hpp"
