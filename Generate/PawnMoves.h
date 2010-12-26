#pragma once
#include "GenerateDriver.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
template<typename, typename> class Propagate;

template<bool Color, typename Rules, typename Board> 
class GenerateDriver<Color, Pieces::PAWN, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Propagate<Rules, Board>&);
        static void generate_promotions(const Position<Board>&, Propagate<Rules, Board>&);

        static size_t count(const Position<Board>&);
        
        static bool detect(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

private:
        // implementation
        static void generate_dirs(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static void generate_dir(BitBoard, Propagate<Rules, Board>&);
        static BitBoard promotors(const Position<Board>&);

        static size_t count_dirs(BitBoard, BitBoard);
        template<size_t> static size_t count_dir(BitBoard, BitBoard);

        static bool detect_dirs(BitBoard, BitBoard);
        template<size_t> static bool detect_dir(BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnMoves.hpp"
