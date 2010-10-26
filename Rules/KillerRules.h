#pragma once
#include "RulesTraits.h"

struct KillerRules {};          // http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules

template<> struct KingCaptureHalt<KillerRules>                  { enum { VALUE = HALT_K }; };
template<> struct is_MajorityPrecedence<KillerRules>            { enum { VALUE = true  }; };
