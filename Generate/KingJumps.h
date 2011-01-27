#pragma once
#include "GenerateDriver.h"
#include "../Board/Board.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;

template<bool Color, typename Rules, typename Board> 
class GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Propagate<Rules, Board>&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

        template<size_t> static bool promotions_en_passant(BitBoard, Propagate<Rules, Board>&);

private:
        // implementation
        
        // tag dispatching based on relative king capture precedence
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&);
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&, Int2Type<true>);
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&, Int2Type<false>);

        static void generate_serial(BitBoard, Propagate<Rules, Board>&);

        // tag dispatching based on king capture directions
        static void generate_dirs(BitBoard, Propagate<Rules, Board>&);
        static void generate_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::DIRS_4>);
        static void generate_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::DIRS_8>);

        // tag dispatching based on king range
        template<size_t> static void generate_dir(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static void generate_dir(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::RANGE_1>);
        template<size_t> static void generate_dir(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::RANGE_N>);

        template<size_t> static void generate_next(BitBoard, Propagate<Rules, Board>&);

        // tag dispatching based on king range
        template<size_t> static bool scan_next(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static bool scan_next(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::RANGE_1>);
        template<size_t> static bool scan_next(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::RANGE_N>);

        // tag dispatching based on king capture direction reversal
        template<size_t> static bool scan_long(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static bool scan_long(BitBoard, Propagate<Rules, Board>&, Int2Type<false>);
        template<size_t> static bool scan_long(BitBoard, Propagate<Rules, Board>&, Int2Type<true>);

        template<size_t> static bool scan_reverse(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static bool scan_forward(BitBoard, Propagate<Rules, Board>&);

        // tag dispatching based on king capture directions
        template<size_t> static bool scan_dirs(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static bool scan_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::DIRS_4>);
        template<size_t> static bool scan_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::DIRS_8>);

        // tag dispatching based on king range
        template<size_t> static bool scan_dir(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static bool scan_dir(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::RANGE_1>);
        template<size_t> static bool scan_dir(BitBoard, Propagate<Rules, Board>&, Int2Type<Variant::RANGE_N>);

        template<size_t> static bool scan(BitBoard, Propagate<Rules, Board>&);

        // tag dispatching based on king capture directions
        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variant::DIRS_4>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variant::DIRS_8>);

        // tag dispatching based on king range
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard);
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<Variant::RANGE_1>);
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<Variant::RANGE_N>);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "KingJumps.hpp"
