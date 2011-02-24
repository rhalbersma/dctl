#pragma once
#include "GenerateTemplate.h"
#include "../Variant/Rules.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"

template<typename> class Position;
namespace Capture { template<typename, typename> class State; }
class Move::List;

template<bool Color, typename Rules, typename Board>
class GenerateTemplate<Color, Pieces::BOTH, Move::JUMPS, Rules, Board>
{
public:
        static void generate(const Position<Board>&, Move::List&);
        static size_t count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation
        static void generate(const Position<Board>&, Capture::State<Rules, Board>&, Move::List&);

        // tag dispatching based on absolute king capture precedence
        static void generate_precede(const Position<Board>&, Capture::State<Rules, Board>&, Move::List&);
        static void generate_precede(const Position<Board>&, Capture::State<Rules, Board>&, Move::List&, Int2Type<false>);
        static void generate_precede(const Position<Board>&, Capture::State<Rules, Board>&, Move::List&, Int2Type<true>);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "BothJumps.hpp"
