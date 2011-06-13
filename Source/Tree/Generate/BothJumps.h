#pragma once
#include "Driver.h"
#include "../Move/Stack.h"
#include "../Node/Pieces.h"
#include "../../Variants/Rules.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"

namespace tree {
namespace generate {

namespace capture { template<typename, typename> class State; }

template<bool Color, typename Rules, typename Board>
class Driver<Color, node::Pieces::BOTH, move::JUMPS, Rules, Board>
{
public:
        static void generate(const node::Position<Board>&, move::Stack*);
        static size_t count(const node::Position<Board>&);
        static bool detect(const node::Position<Board>&);

private:
        // implementation
        static void generate(const node::Position<Board>&, capture::State<Rules, Board>&, move::Stack*);

        // tag dispatching based on absolute king capture precedence
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, move::Stack*);
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, move::Stack*, Int2Type<false>);
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, move::Stack*, Int2Type<true>);
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothJumps.hpp"
