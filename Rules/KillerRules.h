#pragma once
#include "RulesTraits.h"

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
struct KillerRules {};

template<> struct KingCaptureHalt<KillerRules>                  { enum { VALUE = HALT_K }; };
template<> struct is_MajorityPrecedence<KillerRules>            { enum { VALUE = true   }; };
