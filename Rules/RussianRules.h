#pragma once
#include "RulesTraits.h"

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct RussianRules {};

template<> struct is_PromotionEnPassant<RussianRules>           { enum { VALUE = PROMOTE_EP }; };
