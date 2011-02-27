#pragma once
#include "Traits.h"

namespace Variants {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian {};

template<> struct PromotionCondition<Russian>           { enum { VALUE = PROMOTE_EP }; };

}       // namespace Variants
