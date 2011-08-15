#pragma once
#include "Driver.h"
#include "../node/Material.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

namespace capture { template<typename, typename> class State; }

namespace successor {

template<bool Color, typename Rules, typename Board>
class Driver<Color, Material::BOTH, Move::JUMPS, Rules, Board>
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate_regular(const Position<Board>&, Stack&);
        static int count_regular(const Position<Board>&);
        static bool detect_regular(const Position<Board>&);

private:
        // implementation
        static void generate_regular(const Position<Board>&, capture::State<Rules, Board>&, Stack&);

        // tag dispatching based on absolute king capture precedence
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&);
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<false>);
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<true >);
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "BothJumps.hpp"
