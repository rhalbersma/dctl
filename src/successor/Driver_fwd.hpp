#pragma once
#include "Selection.hpp"
#include "../node/Material.hpp"

namespace dctl {
namespace successor {

// primary template
template<bool, int, typename, typename, typename, typename> struct Driver;
/*
// partial specializations on subset selection
template<bool Color, int Material, typename Rules, typename Board> struct Driver<Color, Material, select::Legal, Rules, Board>;
//template<bool Color, int Material, typename Rules, typename Board> struct Driver<Color, Material, select::Conversion, Rules, Board>;

// partial specializations on material composition and subset selection
template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::both, select::Jumps,      Rules, Board>;
template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::both, select::Moves,      Rules, Board>;
//template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::both, select::Reverse,    Rules, Board>;
//template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::both, select::Promotions, Rules, Board>;

template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::king, select::Jumps,      Rules, Board>;
template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::king, select::Moves,      Rules, Board>;
//template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::king, select::Reverse,    Rules, Board>;

template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::pawn, select::Jumps,      Rules, Board>;
template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::pawn, select::Moves,      Rules, Board>;
//template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::pawn, select::Reverse,    Rules, Board>;
//template<bool Color, typename Rules, typename Board> struct Driver<Color, Material::pawn, select::Promotions, Rules, Board>;
*/
}       // namespace successor
}       // namespace dctl
