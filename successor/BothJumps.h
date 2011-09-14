#pragma once
#include "../node/Material.h"
#include "../node/Stack.hpp"
#include "../utility/IntegerTypes.h"
#include "../utility/TemplateTricks.h"

namespace dctl {

namespace capture { template<typename, typename> class State; }

namespace successor {

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::BOTH, Jumps, Rules, Board>
:
        private utility::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);

private:
        // implementation
        static void generate(const Position<Board>&, capture::State<Rules, Board>&, Stack&);

        // tag dispatching on absolute king capture precedence
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&);
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<false>);
        static void generate_precede(const Position<Board>&, capture::State<Rules, Board>&, Stack&, Int2Type<true >);

        // typedefs
        typedef Driver<Color, Material::KING, Jumps, Rules, Board> KingJumps;
        typedef Driver<Color, Material::PAWN, Jumps, Rules, Board> PawnJumps;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "BothJumps.hpp"
