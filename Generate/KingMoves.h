#pragma once
#include "GenerateTemplate.h"
#include "../Variant/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
class MoveList;

template<bool Color, typename Rules, typename Board> 
class GenerateTemplate<Color, Pieces::KING, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Position<Board>&, MoveList&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation
        
        // tag dispatching for restrictions on consecutive moves with the same king
        static void generate_serial(BitBoard, BitBoard, MoveList&);
        static void generate_serial(BitBoard, BitBoard, MoveList&, Int2Type<true>);
        static void generate_serial(BitBoard, BitBoard, MoveList&, Int2Type<false>);
        
        static void generate_dirs(BitBoard, BitBoard, MoveList&);
        
        // tag dispatching based on king range
        template<size_t> static void generate_dir(BitBoard, BitBoard, MoveList&);
        template<size_t> static void generate_dir(BitBoard, BitBoard, MoveList&, Int2Type<Variant::RANGE_1>);
        template<size_t> static void generate_dir(BitBoard, BitBoard, MoveList&, Int2Type<Variant::RANGE_N>);

        static size_t count_dirs(BitBoard, BitBoard);
        
        // tag dispatching based on king range
        template<size_t> static size_t count_dir(BitBoard, BitBoard);
        template<size_t> static size_t count_dir(BitBoard, BitBoard, Int2Type<Variant::RANGE_1>);
        template<size_t> static size_t count_dir(BitBoard, BitBoard, Int2Type<Variant::RANGE_N>);

        static bool detect_dirs(BitBoard, BitBoard);

        template<size_t> static bool detect_dir(BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "KingMoves.hpp"
