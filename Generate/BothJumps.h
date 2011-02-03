#pragma once
#include "GenerateTemplate.h"
#include "../Variant/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
template<typename, typename> class Propagate;
class MoveList;

template<bool Color, typename Rules, typename Board>
class GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, MoveList&);
        static void generate(const Position<Board>&, Propagate<Rules, Board>&, MoveList&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation

        // tag dispatching based on absolute king capture precedence
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&, MoveList&);
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&, MoveList&, Int2Type<false>);
        static void generate_precede(const Position<Board>&, Propagate<Rules, Board>&, MoveList&, Int2Type<true>);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothJumps.hpp"
