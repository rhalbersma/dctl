#pragma once
#include "../node/Material.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"
#include "../rules/Rules.h"

namespace dctl {

namespace board { template<bool, typename> class Direction; }
template<typename> class Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
class Moves;

// partial specialization for king moves
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::KING, Moves, Rules, Board>
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);       
        static bool detect(const Position<Board>&);

private:
        // typedefs
        typedef board::Direction<Color, Board> Direction;
        
        // tag dispatching based on restrictions on consecutive moves with the same king
        static void generate_serial(BitBoard, BitBoard, Stack&);
        static void generate_serial(BitBoard, BitBoard, Stack&, Int2Type<false>);
        static void generate_serial(BitBoard, BitBoard, Stack&, Int2Type<true >);
        
        static void generate_dirs(BitBoard, BitBoard, Stack&);
        
        // tag dispatching based on king range
        template<int> static void generate_dir(BitBoard, BitBoard, Stack&);
        template<int> static void generate_dir(BitBoard, BitBoard, Stack&, Int2Type<rules::RANGE_1>);
        template<int> static void generate_dir(BitBoard, BitBoard, Stack&, Int2Type<rules::RANGE_N>);

        static int count_dirs(BitBoard, BitBoard);
        
        // tag dispatching based on king range
        template<int> static int count_dir(BitBoard, BitBoard);
        template<int> static int count_dir(BitBoard, BitBoard, Int2Type<rules::RANGE_1>);
        template<int> static int count_dir(BitBoard, BitBoard, Int2Type<rules::RANGE_N>);

        static bool detect_dirs(BitBoard, BitBoard);
        template<int> static bool detect_dir(BitBoard, BitBoard);
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "KingMoves.hpp"
