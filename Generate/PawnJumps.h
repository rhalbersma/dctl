#pragma once
#include "GenerateDriver.h"
#include "../Rules/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;

template<bool Color, typename Rules, typename Board> 
class GenerateDriver<Pieces::PAWN, Move::JUMPS, Color, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Propagate<Rules, Board>&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation
        static void generate_targets(const Position<Board>&, Propagate<Rules, Board>&);
        static void generate_targets(const Position<Board>&, Propagate<Rules, Board>&, Int2Type<false>);
        static void generate_targets(const Position<Board>&, Propagate<Rules, Board>&, Int2Type<true>);

        static void generate_dirs(BitBoard, Propagate<Rules, Board>&);
        static void generate_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<DIRS_2>);
        static void generate_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<DIRS_4>);
        static void generate_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<DIRS_8>);

        template<size_t> static void generate_dir(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static void generate_next(BitBoard, Propagate<Rules, Board>&);

        template<size_t> static bool scan_next(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static bool scan_next(BitBoard, Propagate<Rules, Board>&, Int2Type<PROMOTE_BR>);
        template<size_t> static bool scan_next(BitBoard, Propagate<Rules, Board>&, Int2Type<PROMOTE_EP>);

        template<size_t> static bool scan_dirs(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static bool scan_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<DIRS_2>);
        template<size_t> static bool scan_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<DIRS_4>);
        template<size_t> static bool scan_dirs(BitBoard, Propagate<Rules, Board>&, Int2Type<DIRS_8>);

        template<size_t> static bool scan_dir(BitBoard, Propagate<Rules, Board>&);
        template<size_t> static bool scan(BitBoard, Propagate<Rules, Board>&);

        static bool detect_targets(const Position<Board>&);
        static bool detect_targets(const Position<Board>&, Int2Type<false>);
        static bool detect_targets(const Position<Board>&, Int2Type<true>);

        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<DIRS_2>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<DIRS_4>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<DIRS_8>);

        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnJumps.hpp"
