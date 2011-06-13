#pragma once
#include "Driver.h"
#include "../Move/Stack.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Variants/Rules.h"

namespace tree {

namespace node { template<typename> class Position; }

namespace generate {

namespace capture { template<typename, typename> class State; }

template<bool Color, typename Rules, typename Board> 
class Driver<Color, node::Pieces::KING, move::JUMPS, Rules, Board>
{
public:
        static void generate(const node::Position<Board>&, move::Stack*);
        static void generate(const node::Position<Board>&, capture::State<Rules, Board>&, move::Stack*);
        static size_t count(const node::Position<Board>&);
        static bool detect(const node::Position<Board>&);

        template<size_t> static bool promote_en_passant(BitBoard, capture::State<Rules, Board>&, move::Stack*);

private:
        // implementation
        
        // tag dispatching based on relative king capture precedence
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, move::Stack*);
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, move::Stack*, Int2Type<true >);
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, move::Stack*, Int2Type<false>);

        static void generate_serial(BitBoard, capture::State<Rules, Board>&, move::Stack*);

        // tag dispatching based on king capture directions
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, move::Stack*);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<variants::DIRS_ALL >);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<variants::DIRS_ORTH>);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<variants::DIRS_DIAG>);

        template<size_t> static void generate_dir(BitBoard, capture::State<Rules, Board>&, move::Stack*);

        template<size_t> static void generate_next(BitBoard, capture::State<Rules, Board>&, move::Stack*);

        // tag dispatching based on king range
        template<size_t> static bool scan_next(BitBoard, capture::State<Rules, Board>&, move::Stack*);
        template<size_t> static bool scan_next(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<variants::RANGE_1>);
        template<size_t> static bool scan_next(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<variants::RANGE_N>);

        // tag dispatching based on king capture direction reversal
        template<size_t> static bool scan_long(BitBoard, capture::State<Rules, Board>&, move::Stack*);
        template<size_t> static bool scan_long(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<false>);
        template<size_t> static bool scan_long(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<true >);

        template<size_t> static bool scan_reverse(BitBoard, capture::State<Rules, Board>&, move::Stack*);
        template<size_t> static bool scan_forward(BitBoard, capture::State<Rules, Board>&, move::Stack*);

        // tag dispatching based on king scan directions
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, move::Stack*);
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<variants::SCAN_ALL >);
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<variants::SCAN_SIDE>);
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, move::Stack*, Int2Type<variants::SCAN_REST>);

        template<size_t> static bool scan_dir(BitBoard, capture::State<Rules, Board>&, move::Stack*);
        template<size_t> static bool scan(BitBoard, capture::State<Rules, Board>&, move::Stack*);

        // tag dispatching based on king range
        template<size_t> static void king_slide(BitBoard&, BitBoard);
        template<size_t> static void king_slide(BitBoard&, BitBoard, Int2Type<variants::RANGE_1>);
        template<size_t> static void king_slide(BitBoard&, BitBoard, Int2Type<variants::RANGE_N>);

        // tag dispatching based on king capture directions
        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<variants::DIRS_ALL >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<variants::DIRS_ORTH>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<variants::DIRS_DIAG>);

        // tag dispatching based on king range
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard);
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<variants::RANGE_1>);
        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<variants::RANGE_N>);
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "KingJumps.hpp"
