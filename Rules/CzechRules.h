#pragma once
#include "RulesTraits.h"

// http://www.damweb.cz/pravidla/cdfull.html
struct CzechRules {};

template<> struct ManCaptureDirections<CzechRules>              { enum { VALUE = DIRS_2 }; };
template<> struct is_AbsoluteKingPrecedence<CzechRules>         { enum { VALUE = true   }; };
