#pragma once
#include "../../Variants/Rules.h"

namespace Tree {
namespace Move {

struct NUM_tag {};      // numeric
struct ALG_tag {};      // algebraic

template<typename> struct Protocol		{ typedef NUM_tag TYPE; };
template<> struct Protocol<Variants::Russian>   { typedef ALG_tag TYPE; };
template<> struct Protocol<Variants::Czech>	{ typedef ALG_tag TYPE; };

}       // namespace Move
}       // namespace Tree
