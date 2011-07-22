#pragma once
#include "../rules/Variants.h"

namespace dctl {
namespace move {

struct NUM_tag {};      // numeric
struct ALG_tag {};      // algebraic

template<typename> struct Protocol		{ typedef NUM_tag type; };
template<> struct Protocol<rules::Russian>      { typedef ALG_tag type; };
template<> struct Protocol<rules::Czech>	{ typedef ALG_tag type; };

}       // namespace move
}       // namespace dctl
