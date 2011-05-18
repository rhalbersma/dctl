#pragma once
#include "Template.h"
#include "../Move/Types.h"
#include "../../Utilities/IntegerTypes.h"

namespace Tree {

namespace Node { template<typename> class Position; }
namespace Move { class Stack; }

namespace Generate {

template<bool Color, typename Rules, typename Board> 
class Template<Color, Node::Pieces::PAWN, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Node::Position<Board>&, Move::Stack&);
        static void generate_promotions(const Node::Position<Board>&, Move::Stack&);

        static size_t count(const Node::Position<Board>&);
        static size_t count_promotions(const Node::Position<Board>&);
        
        static bool detect(const Node::Position<Board>&);
        static bool detect_promotions(const Node::Position<Board>&);

private:
        // implementation
        static void generate_dirs(BitBoard, BitBoard, Move::Stack&);
        template<size_t> static void generate_dir(BitBoard, BitBoard, Move::Stack&);

        static size_t count_dirs(BitBoard, BitBoard);
        template<size_t> static size_t count_dir(BitBoard, BitBoard);

        static bool detect_dirs(BitBoard, BitBoard);
        template<size_t> static bool detect_dir(BitBoard, BitBoard);

        static BitBoard promotion(BitBoard);
        static BitBoard promotors(BitBoard);
};

}       // namespace Generate
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PawnMoves.hpp"
