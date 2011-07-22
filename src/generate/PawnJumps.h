#pragma once
#include "Driver.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"
#include "../rules/Rules.h"

namespace dctl {

template<typename> class Position;

namespace generate {

namespace capture { template<typename, typename> class State; }

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Pieces::PAWN, JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Stack&);
        static void generate(const Position<Board>&, capture::State<Rules, Board>&, Stack&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation
        
        // tag dispatching based on whether men can capture kings
        static void generate_targets(const Position<Board>&, capture::State<Rules, Board>&, Stack&);
        static void generate_targets(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<false>);
        static void generate_targets(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<true>);

        // tag dispatching based on man capture directions
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::DIRS_ALL >);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::DIRS_ORTH>);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::DIRS_DIAG>);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::DIRS_UP  >);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::DIRS_DOWN>);

        template<size_t> static void generate_dir(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<size_t> static void generate_next(BitBoard, capture::State<Rules, Board>&, Stack&);

        // tag dispatching based on promotion condition
        template<size_t> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<size_t> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::PROMOTE_BR>);
        template<size_t> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::PROMOTE_EP>);

        // tag dispatching based on man scan directions
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::SCAN_ALL >);
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::SCAN_REST>);
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::SCAN_SIDE>);
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::SCAN_UP  >);
        template<size_t> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::SCAN_DOWN>);

        template<size_t> static bool scan_dir(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<size_t> static bool scan(BitBoard, capture::State<Rules, Board>&, Stack&);

        // tag dispatching based on whether men can capture kings
        static bool detect_targets(const Position<Board>&);
        static bool detect_targets(const Position<Board>&, Int2Type<false>);
        static bool detect_targets(const Position<Board>&, Int2Type<true>);

        // tag dispatching based on man capture directions
        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::DIRS_ALL >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::DIRS_ORTH>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::DIRS_DIAG>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::DIRS_UP  >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::DIRS_DOWN>);

        template<size_t> static bool detect_dir(BitBoard, BitBoard, BitBoard);
};

}       // namespace generate
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnJumps.hpp"
