#pragma once
#include "Driver.h"
#include "Selection.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

namespace board { template<bool, typename> class Direction; }
template<typename> class Position;

namespace successor {

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Moves, Rules, Board>
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

        // typedefs
        typedef board::Direction<Color, Board> Direction;

        // implementation
        static void generate_dirs(BitBoard, BitBoard, Stack&);
        template<int> static void generate_dir(BitBoard, BitBoard, Stack&);

        static int count_dirs(BitBoard, BitBoard);
        template<int> static int count_dir(BitBoard, BitBoard);

        static bool detect_dirs(BitBoard, BitBoard);
        template<int> static bool detect_dir(BitBoard, BitBoard);
};

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Reverse, Rules, Board>
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // typedefs
        typedef Driver<Color, Material::PAWN, Moves, Rules, Board> Regular;
        typedef board::Direction<Color, Board> Direction;

        // implementation
        static void generate_dirs(BitBoard, BitBoard, Stack&);
        static int count_dirs(BitBoard, BitBoard);
        static bool detect_dirs(BitBoard, BitBoard);
};

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Promotion, Rules, Board>
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // typedefs
        typedef Driver<Color, Material::PAWN, Moves, Rules, Board> Regular;
        typedef board::Direction<Color, Board> Direction;
};

// implementation
template<bool, typename> BitBoard promotion(BitBoard);
template<bool, typename> BitBoard promotors(BitBoard);

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "PawnMoves.hpp"
