#pragma once
#include "Template.h"
#include "../Variants/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
namespace Capture { template<typename, typename> class State; }
namespace Move { class Stack; }

template<bool Color, typename Rules, typename Board> 
class Template<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Move::Stack&);
        static void generate(const Position<Board>&, Capture::State<Rules, Board>&, Move::Stack&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation
        
        // tag dispatching based on whether men can capture kings
        static void generate_targets(const Position<Board>&, Capture::State<Rules, Board>&, Move::Stack&);
        static void generate_targets(const Position<Board>&, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<false>);
        static void generate_targets(const Position<Board>&, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<true>);

        // tag dispatching based on man capture directions
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&);
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::DIRS_ALL >);
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::DIRS_ORTH>);
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::DIRS_DIAG>);
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::DIRS_UP  >);
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::DIRS_DOWN>);

        template<size_t> static void generate_dir(BitBoard, Capture::State<Rules, Board>&, Move::Stack&);
        template<size_t> static void generate_next(BitBoard, Capture::State<Rules, Board>&, Move::Stack&);

        // tag dispatching based on promotion condition
        template<size_t> static bool scan_next(BitBoard, Capture::State<Rules, Board>&, Move::Stack&);
        template<size_t> static bool scan_next(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::PROMOTE_BR>);
        template<size_t> static bool scan_next(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::PROMOTE_EP>);

        // tag dispatching based on man scan directions
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&);
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::SCAN_ALL >);
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::SCAN_REST>);
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::SCAN_SIDE>);
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::SCAN_UP  >);
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<Variants::SCAN_DOWN>);

        template<size_t> static bool scan_dir(BitBoard, Capture::State<Rules, Board>&, Move::Stack&);
        template<size_t> static bool scan(BitBoard, Capture::State<Rules, Board>&, Move::Stack&);

        // tag dispatching based on whether men can capture kings
        static bool detect_targets(const Position<Board>&);
        static bool detect_targets(const Position<Board>&, Int2Type<false>);
        static bool detect_targets(const Position<Board>&, Int2Type<true>);

        // tag dispatching based on man capture directions
        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variants::DIRS_ALL >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variants::DIRS_ORTH>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variants::DIRS_DIAG>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variants::DIRS_UP  >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variants::DIRS_DOWN>);

        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnJumps.hpp"
