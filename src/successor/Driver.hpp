#include "BothJumps.h"
#include "BothMoves.h"
#include "KingJumps.h"
#include "KingMoves.h"
#include "PawnJumps.h"
#include "PawnMoves.h"
#include "../node/Material.h"

namespace dctl {
namespace successor {

template<bool Color, int Material, int MoveType, typename Rules, typename Board>
void Driver<Color, Material, MoveType, Rules, Board>::generate(const Position<Board>&, Stack&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return;
}

template<bool Color, int Material, int MoveType, typename Rules, typename Board>
void Driver<Color, Material, MoveType, Rules, Board>::generate_reverse(const Position<Board>&, Stack&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return;
}

template<bool Color, int Material, int MoveType, typename Rules, typename Board>
void Driver<Color, Material, MoveType, Rules, Board>::generate_promotions(const Position<Board>&, Stack&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return;
}

template<bool Color, int Material, int MoveType, typename Rules, typename Board>
int Driver<Color, Material, MoveType, Rules, Board>::count(const Position<Board>&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return 0; 
}
        
template<bool Color, int Material, int MoveType, typename Rules, typename Board>
int Driver<Color, Material, MoveType, Rules, Board>::count_reverse(const Position<Board>&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return 0; 
}

template<bool Color, int Material, int MoveType, typename Rules, typename Board>
int Driver<Color, Material, MoveType, Rules, Board>::count_promotions(const Position<Board>&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return 0; 
}

template<bool Color, int Material, int MoveType, typename Rules, typename Board>
bool Driver<Color, Material, MoveType, Rules, Board>::detect(const Position<Board>&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return false; 
}

template<bool Color, int Material, int MoveType, typename Rules, typename Board>
bool Driver<Color, Material, MoveType, Rules, Board>::detect_reverse(const Position<Board>&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return false; 
}

template<bool Color, int Material, int MoveType, typename Rules, typename Board>
bool Driver<Color, Material, MoveType, Rules, Board>::detect_promotions(const Position<Board>&) 
{ 
        static_assert(Material == Material::NONE, "Template function already defined.");
        return false; 
}

}       // namespace successor
}       // namespace dctl
