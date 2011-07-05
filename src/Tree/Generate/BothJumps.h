#pragma once
#include "Driver.h"
#include "../Node/Pieces.h"
#include "../Node/Stack.h"
#include "../../rules/Traits.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"

namespace tree {
namespace generate {

namespace capture { template<typename, typename> class State; }

template<bool Color, typename Rules, typename Board>
class Driver<Color, node::Pieces::BOTH, node::JUMPS, Rules, Board>
{
public:
        static void generate(const node::Position<Board>&, node::Stack&);
        static size_t count(const node::Position<Board>&);
        static bool detect(const node::Position<Board>&);

private:
        // implementation
        static void generate(const node::Position<Board>&, capture::State<Rules, Board>&, node::Stack&);

        // tag dispatching based on absolute king capture precedence
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, node::Stack&);
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, node::Stack&, Int2Type<false>);
        static void generate_precede(const node::Position<Board>&, capture::State<Rules, Board>&, node::Stack&, Int2Type<true>);
};

}       // namespace generate
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothJumps.hpp"
