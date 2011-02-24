#pragma once
#include "GenerateState.h"
#include "../Position/Pieces.h"
#include "../Position/Side.h"
#include <cstddef>

template<typename> class Position;
namespace Move { class List; }
        
template<typename Rules, typename Board>
class Generate
{
public:
        // interface
        static void generate(const Position<Board>&, Move::List&);
        static void generate_captures(const Position<Board>&, Move::List&);
        static void generate_promotions(const Position<Board>&, Move::List&);

        static size_t count(const Position<Board>&);
        template<bool> static size_t count_mobility(const Position<Board>&);

        static bool detect(const Position<Board>&);
        static bool detect_captures(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

private:
        // implementation
        static const AbstractGenerateState<Rules, Board>* select(const Position<Board>&);
        template<bool> static const AbstractGenerateState<Rules, Board>* select(const Position<Board>&);

        static int state(const Position<Board>&);
        template<bool> static int state(const Position<Board>&);
        static int state(bool, BitBoard, BitBoard);

        static bool invariant(const Position<Board>&, size_t);

        // representation
        static const AbstractGenerateState<Rules, Board>* const STATE[];

        static const GenerateState<Side::BLACK, Pieces::NONE, Rules, Board> BLACK_NONE;
        static const GenerateState<Side::BLACK, Pieces::PAWN, Rules, Board> BLACK_PAWN;
        static const GenerateState<Side::BLACK, Pieces::KING, Rules, Board> BLACK_KING;
        static const GenerateState<Side::BLACK, Pieces::BOTH, Rules, Board> BLACK_BOTH;
        static const GenerateState<Side::WHITE, Pieces::NONE, Rules, Board> WHITE_NONE;
        static const GenerateState<Side::WHITE, Pieces::PAWN, Rules, Board> WHITE_PAWN;
        static const GenerateState<Side::WHITE, Pieces::KING, Rules, Board> WHITE_KING;
        static const GenerateState<Side::WHITE, Pieces::BOTH, Rules, Board> WHITE_BOTH;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Generate.hpp"
