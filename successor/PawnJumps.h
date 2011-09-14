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

// partial specialization for pawn jumps
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Jumps, Rules, Board>
:
        private utility::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

        static void generate(const Position<Board>&, capture::State<Rules, Board>&, Stack&);

private:
        // typedefs
        typedef Driver<Color, Material::KING, Jumps, Rules, Board> KingJumps;
        typedef board::Direction<Color, Board> Direction;
        
        // tag dispatching on whether men can capture kings
        static void generate_targets(const Position<Board>&, capture::State<Rules, Board>&, Stack&);
        static void generate_targets(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<false>);
        static void generate_targets(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<true >);

        // tag dispatching on man capture directions
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::dirs_all >);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::dirs_orth>);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::dirs_diag>);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::dirs_up  >);
        static void generate_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::dirs_down>);

        template<int> static void generate_dir(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static void generate_next(BitBoard, capture::State<Rules, Board>&, Stack&);

        // tag dispatching on promotion condition
        template<int> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::promote_af>);
        template<int> static bool scan_next(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::promote_ep>);

        // tag dispatching on man scan directions
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::scan_all >);
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::scan_orth>);
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::scan_diag>);
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::scan_up  >);
        template<int> static bool scan_dirs(BitBoard, capture::State<Rules, Board>&, Stack&, Int2Type<rules::scan_down>);

        template<int> static bool scan_dir(BitBoard, capture::State<Rules, Board>&, Stack&);
        template<int> static bool scan(BitBoard, capture::State<Rules, Board>&, Stack&);

        // tag dispatching on whether men can capture kings
        static bool detect_targets(const Position<Board>&);
        static bool detect_targets(const Position<Board>&, Int2Type<false>);
        static bool detect_targets(const Position<Board>&, Int2Type<true>);

        // tag dispatching on man capture directions
        static bool detect_dirs(BitBoard, BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::dirs_all >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::dirs_orth>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::dirs_diag>);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::dirs_up  >);
        static bool detect_dirs(BitBoard, BitBoard, BitBoard, Int2Type<rules::dirs_down>);

        template<int> static bool detect_dir(BitBoard, BitBoard, BitBoard);
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "PawnJumps.hpp"
