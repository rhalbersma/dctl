#pragma once
#include "RulesTraits.h"

struct CzechRules {};           // http://www.damweb.cz/pravidla/cdfull.html

template<> struct ManCaptureDirections<CzechRules>              { static const size_t VALUE = DIRS_2; };
template<> struct is_AbsoluteKingPrecedence<CzechRules>         { enum { VALUE = true  }; };
