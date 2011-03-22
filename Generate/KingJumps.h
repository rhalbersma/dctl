#pragma once
#include "Template.h"
#include "../Geometry/Board.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
namespace Capture { template<typename, typename> class State; }
namespace Move { class List; }

template<bool Color, typename Rules, typename Board> 
class Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Move::List&);
        static void generate(const Position<Board>&, Capture::State<Rules, Board>&, Move::List&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

        template<size_t> static bool promote_en_passant(BitBoard, Capture::State<Rules, Board>&, Move::List&);

private:
        // implementation
        
        // tag dispatching based on relative king capture precedence
        static void generate_precede(const Position<Board>&, Capture::State<Rules, Board>&, Move::List&);
        static void generate_precede(const Position<Board>&, Capture::State<Rules, Board>&, Move::List&, Int2Type<true >);
        static void generate_precede(const Position<Board>&, Capture::State<Rules, Board>&, Move::List&, Int2Type<false>);

        static void generate_serial(BitBoard, Capture::State<Rules, Board>&, Move::List&);

        // tag dispatching based on king capture directions
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::List&);
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<Variants::DIRS_ALL >);
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<Variants::DIRS_ORTH>);
        static void generate_dirs(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<Variants::DIRS_DIAG>);

        template<size_t> static void generate_dir(BitBoard, Capture::State<Rules, Board>&, Move::List&);

        template<size_t> static void generate_next(BitBoard, Capture::State<Rules, Board>&, Move::List&);

        // tag dispatching based on king range
        template<size_t> static bool scan_next(BitBoard, Capture::State<Rules, Board>&, Move::List&);
        template<size_t> static bool scan_next(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<Variants::RANGE_1>);
        template<size_t> static bool scan_next(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<Variants::RANGE_N>);

        // tag dispatching based on king capture direction reversal
        template<size_t> static bool scan_long(BitBoard, Capture::State<Rules, Board>&, Move::List&);
        template<size_t> static bool scan_long(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<false>);
        template<size_t> static bool scan_long(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<true >);

        template<size_t> static bool scan_reverse(BitBoard, Capture::State<Rules, Board>&, Move::List&);
        template<size_t> static bool scan_forward(BitBoard, Capture::State<Rules, Board>&, Move::List&);

        // tag dispatching based on king scan directions
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::List&);
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<Variants::SCAN_ALL >);
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<Variants::SCAN_SIDE>);
        template<size_t> static bool scan_dirs(BitBoard, Capture::State<Rules, Board>&, Move::List&, Int2Type<Variants::SCAN_REST>);

        template<size_t> static bool scan_dir(BitBoard, Capture::State<Rules, Board>&, Move::List&);
        template<size_t> static bool scan(BitBoard, Capture::State<Rules, Board>&, Move::List&);

        // tag dispatching based on king range
        template<size_t> static void king_slide(BitBoard&, BitBoard);
        template<size_t> static void king_slide(BitBoard&, BitBoard, Int2Type<Variants::RANGE_1>);
        template<size_t> static void king_slide(BitBoard&, BitBoard, Int2Type<Variants::RANGE_N>);

        // tag dispatching based on king capture directions
        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variants::DIRS_ALL >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variants::DIRS_ORTH>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<Variants::DIRS_DIAG>);

        // tag dispatching based on king range
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard);
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<Variants::RANGE_1>);
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<Variants::RANGE_N>);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "KingJumps.hpp"
