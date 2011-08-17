#include "BothJumps.h"
#include "BothMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "PawnJumps.h"
#include "PawnMoves.h"
#include "../node/Material.h"

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Rules, typename Board>
void Driver<Color, Material, Selection, Rules, Board>::generate(const Position<Board>&, Stack&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return;
}

template<bool Color, int Material, typename Selection, typename Rules, typename Board>
int Driver<Color, Material, Selection, Rules, Board>::count(const Position<Board>&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return 0; 
}

template<bool Color, int Material, typename Selection, typename Rules, typename Board>
bool Driver<Color, Material, Selection, Rules, Board>::detect(const Position<Board>&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return false; 
}

}       // namespace successor
}       // namespace dctl
