#pragma once
#include "Template.h"
#include "../Move/Types.h"
#include "../../Utilities/IntegerTypes.h"

namespace Tree {

namespace Node { template<typename> class Position; }
namespace Move { class Stack; }

namespace Generate {

template<bool Color, typename Rules, typename Board> 
class Template<Color, Node::Pieces::BOTH, Move::MOVES, Rules, Board>
{
public:
        static void generate(const Node::Position<Board>&, Move::Stack&);
        static size_t count(const Node::Position<Board>&);        
        static bool detect(const Node::Position<Board>&);
};

}       // namespace Generate
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothMoves.hpp"
