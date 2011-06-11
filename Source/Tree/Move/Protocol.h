#pragma once
#include "../../Variants/Rules.h"

namespace tree {
namespace move {

struct NUM_tag {};      // numeric
struct ALG_tag {};      // algebraic

template<typename> struct Protocol		{ typedef NUM_tag TYPE; };
template<> struct Protocol<variants::Russian>   { typedef ALG_tag TYPE; };
template<> struct Protocol<variants::Czech>	{ typedef ALG_tag TYPE; };

}       // namespace move
}       // namespace tree
