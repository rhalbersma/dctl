#pragma once
#include "../node/Material.h"
#include "../node/Stack.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"

namespace dctl {

template<typename> class Position;

namespace successor {

template<bool Color, typename Selection, typename Rules, typename Board> 
 
class Driver<Color, Material::BOTH, Selection, Rules, Board>
:
        private utils::nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>&, Stack&);
        static int count(const Position<Board>&);
        static bool detect(const Position<Board>&);
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "BothMoves.hpp"