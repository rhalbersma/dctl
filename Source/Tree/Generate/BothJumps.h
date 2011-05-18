#pragma once
#include "Template.h"
#include "../Move/Types.h"
#include "../Node/Pieces.h"
#include "../../Variants/Rules.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"

namespace Tree {

namespace Move { class Stack; }

namespace Generate {

namespace Capture { template<typename, typename> class State; }

template<bool Color, typename Rules, typename Board>
class Template<Color, Node::Pieces::BOTH, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Node::Position<Board>&, Move::Stack&);
        static size_t count(const Node::Position<Board>&);
        static bool detect(const Node::Position<Board>&);

private:
        // implementation
        static void generate(const Node::Position<Board>&, Capture::State<Rules, Board>&, Move::Stack&);

        // tag dispatching based on absolute king capture precedence
        static void generate_precede(const Node::Position<Board>&, Capture::State<Rules, Board>&, Move::Stack&);
        static void generate_precede(const Node::Position<Board>&, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<false>);
        static void generate_precede(const Node::Position<Board>&, Capture::State<Rules, Board>&, Move::Stack&, Int2Type<true>);
};

}       // namespace Generate
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothJumps.hpp"
