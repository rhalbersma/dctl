#pragma once
#include "../node/Material.h"
#include "../node/Stack.hpp"
#include "../utility/IntegerTypes.h"
#include "../utility/TemplateTricks.h"
#include "../rules/Rules.h"

namespace dctl {

namespace board { template<bool, typename> class Direction; }
namespace capture { template<typename, typename> class State; }
template<typename> class Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
class Jumps;

// partial specialization for king jumps
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::KING, Jumps, Rules, Board>
:
        private utility::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

        static void generate(const Position<Board>&, capture::State<Rules, Board>&, Stack&);
        template<int> static bool promote_en_passant(BitBoard, capture::State<Rules, Board>&, Stack&);

private:
        // tag dispatching on relative king capture precedence
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&);
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<true >);
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<false>);

        static void generate_serial(BitBoard, capture::State<Rules, Board>&, Stack&);

        // tag dispatching on king capture directions
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::dirs_all >);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::dirs_orth>);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::dirs_diag>);

        template<int> static void generate_dir(BitBoard, capture::State<Rules, Board>&, Stack&);

        template<int> static void generate_next(BitBoard, capture::State<Rules, Board>&, Stack&);

        // tag dispatching on king range
        template<int> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::range_1>);
        template<int> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::range_N>);

        // tag dispatching on king capture direction reversal
        template<int> static bool scan_long(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static bool scan_long(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<false>);
        template<int> static bool scan_long(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<true >);

        template<int> static bool scan_reverse(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static bool scan_forward(BitBoard, capture::State<Rules, Board>&, Stack&);

        // tag dispatching on king scan directions
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::scan_all >);
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::scan_diag>);
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::scan_orth>);

        template<int> static bool scan_dir(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static bool scan(BitBoard, capture::State<Rules, Board>&, Stack&);

        // tag dispatching on king range
        template<int> static void king_slide(BitBoard&, BitBoard);
        template<int> static void king_slide(BitBoard&, BitBoard, Int2Type<rules::range_1>);
        template<int> static void king_slide(BitBoard&, BitBoard, Int2Type<rules::range_N>);

        // tag dispatching on king capture directions
        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::dirs_all >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::dirs_orth>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::dirs_diag>);

        // tag dispatching on king range
        template<int> static bool detect_dir(BitBoard, BitBoard, BitBoard);
        template<int> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<rules::range_1>);
        template<int> static bool detect_dir(BitBoard, BitBoard, BitBoard, Int2Type<rules::range_N>);

        // typedefs
        typedef board::Direction<Color, Board> Direction;        
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "KingJumps.hpp"
