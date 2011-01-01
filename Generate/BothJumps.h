#pragma once
#include "GenerateDriver.h"
#include "../Rules/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;

template<bool Color, typename Rules, typename Board>
class GenerateDriver<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Propagate<Rules, Board>&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation

        // tag dispatching based on absolute king capture precedence
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&);
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&, Int2Type<false>);
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&, Int2Type<true>);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothJumps.hpp"
