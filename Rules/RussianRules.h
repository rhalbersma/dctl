#pragma once
#include "RulesTraits.h"

struct RussianRules {};         // http://www.shashist.ru/kodeks/kodeks2004.doc

template<> struct is_PromotionEnPassant<RussianRules>           { enum { VALUE = PROMOTE_EP }; };
