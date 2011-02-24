#pragma once
#include "GenerateTemplate.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
class Move::List;

template<bool Color, typename Rules, typename Board> 
class GenerateTemplate<Color, Pieces::PAWN, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Move::List&);
        static void generate_promotions(const Position<Board>&, Move::List&);

        static size_t count(const Position<Board>&);
        
        static bool detect(const Position<Board>&);
        static bool detect_promotions(const Position<Board>&);

private:
        // implementation
        static void generate_dirs(BitBoard, BitBoard, Move::List&);
        template<size_t> static void generate_dir(BitBoard, BitBoard, Move::List&);

        static BitBoard promotion(BitBoard);
        static BitBoard promotors(BitBoard);

        static size_t count_dirs(BitBoard, BitBoard);
        template<size_t> static size_t count_dir(BitBoard, BitBoard);

        static bool detect_dirs(BitBoard, BitBoard);
        template<size_t> static bool detect_dir(BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnMoves.hpp"
