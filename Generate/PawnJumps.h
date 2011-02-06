#pragma once
#include "GenerateTemplate.h"
#include "../Variant/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Capture;

template<bool Color, typename Rules, typename Board> 
class GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, MoveList&);
        static void generate(const Position<Board>&, Capture<Rules, Board>&, MoveList&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation
        
        // tag dispatching based on whether men can capture kings
        static void generate_targets(const Position<Board>&, Capture<Rules, Board>&, MoveList&);
        static void generate_targets(const Position<Board>&, Capture<Rules, Board>&, MoveList&, Int2Type<false>);
        static void generate_targets(const Position<Board>&, Capture<Rules, Board>&, MoveList&, Int2Type<true>);

        // tag dispatching based on man capture directions
        static void generate_dirs(BitBoard, Capture<Rules, Board>&, MoveList&);
        static void generate_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_2>);
        static void generate_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_4>);
        static void generate_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_8>);

        template<size_t> static void generate_dir(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static void generate_next(BitBoard, Capture<Rules, Board>&, MoveList&);

        // tag dispatching based on promotion condition
        template<size_t> static bool scan_next(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static bool scan_next(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::PROMOTE_BR>);
        template<size_t> static bool scan_next(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::PROMOTE_EP>);

        // tag dispatching based on man capture directions
        template<size_t> static bool scan_dirs(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static bool scan_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_2>);
        template<size_t> static bool scan_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_4>);
        template<size_t> static bool scan_dirs(BitBoard, Capture<Rules, Board>&, MoveList&, Int2Type<Variant::DIRS_8>);

        template<size_t> static bool scan_dir(BitBoard, Capture<Rules, Board>&, MoveList&);
        template<size_t> static bool scan(BitBoard, Capture<Rules, Board>&, MoveList&);

        // tag dispatching based on whether men can capture kings
        static bool detect_targets(const Position<Board>&);
        static bool detect_targets(const Position<Board>&, Int2Type<false>);
        static bool detect_targets(const Position<Board>&, Int2Type<true>);

        // tag dispatching based on man capture directions
        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variant::DIRS_2>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variant::DIRS_4>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variant::DIRS_8>);

        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnJumps.hpp"
