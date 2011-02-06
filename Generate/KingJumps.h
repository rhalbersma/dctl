#pragma once
#include "GenerateTemplate.h"
#include "../Board/Board.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Capture;
class MoveList;

template<bool Color, typename Rules, typename Board> 
class GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, MoveList&);
        static void generate(const Position<Board>&, Capture<Rules, Board>&, MoveList&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

        template<size_t> static bool promote_en_passant(BitBoard, Capture<Rules, Board>&, MoveList&);

private:
        // implementation
        
        // tag dispatching based on relative king capture precedence
        static void generate_precede(const Position<Board>&, Capture<Rules, Board>&, MoveList&);
        static void generate_precede(const Position<Board>&, Capture<Rules, Board>&, MoveList&, Int2Type<true>);
        static void generate_precede(const Position<Board>&, Capture<Rules, Board>&, MoveList&, Int2Type<false>);

        static void generate_serial(BitBoard, Capture<Rules, Board>&, MoveList&);

        // tag dispatching based on king capture directions
        static void generate_dirs(BitBoard, Capture<Rules, Board>&, MoveList&);
        static void generate_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_4>);
        static void generate_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_8>);

        // tag dispatching based on king range
        template<size_t> static void generate_dir(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static void generate_dir(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::RANGE_1>);
        template<size_t> static void generate_dir(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::RANGE_N>);

        template<size_t> static void generate_next(BitBoard, Capture<Rules, Board>&, MoveList&);

        // tag dispatching based on king range
        template<size_t> static bool scan_next(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static bool scan_next(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::RANGE_1>);
        template<size_t> static bool scan_next(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::RANGE_N>);

        // tag dispatching based on king capture direction reversal
        template<size_t> static bool scan_long(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static bool scan_long(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<false>);
        template<size_t> static bool scan_long(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<true>);

        template<size_t> static bool scan_reverse(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static bool scan_forward(BitBoard, Capture<Rules, Board>&, MoveList&);

        // tag dispatching based on king capture directions
        template<size_t> static bool scan_dirs(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static bool scan_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_4>);
        template<size_t> static bool scan_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_8>);

        // tag dispatching based on king range
        template<size_t> static bool scan_dir(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static bool scan_dir(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::RANGE_1>);
        template<size_t> static bool scan_dir(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::RANGE_N>);

        template<size_t> static bool scan(BitBoard, Capture<Rules, Board>&, MoveList&);

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
