#pragma once
#include "Traits.h"

namespace Variant {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian {};

template<> struct PromotionCondition<Russian>           { enum { VALUE = PROMOTE_EP }; };

}       // namespace Variant
