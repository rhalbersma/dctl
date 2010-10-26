#pragma once
#include "RulesTraits.h"

struct ThaiRules {};

template<> struct ManCaptureDirections<ThaiRules>               { static const size_t VALUE = DIRS_2; };
template<> struct KingCaptureHalt<ThaiRules>                    { enum { VALUE = HALT_1 }; };
template<> struct is_IntegralCaptureRemoval<ThaiRules>          { enum { VALUE = REMOVE_1 }; };
template<> struct is_DirectionReversal<ThaiRules>               { enum { VALUE = true  }; };
template<> struct LargeCaptureValue<ThaiRules>                  { enum { VALUE = 3 }; };
