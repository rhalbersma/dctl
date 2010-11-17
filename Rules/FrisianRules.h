#pragma once
#include "RulesTraits.h"

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct FrisianRules {};

template<> struct ManCaptureDirections<FrisianRules>            { enum { VALUE = DIRS_8 }; };
template<> struct KingCaptureDirections<FrisianRules>           { enum { VALUE = DIRS_8 }; };
template<> struct LargeCaptureValue<FrisianRules>               { enum { VALUE = 3      }; };
template<> struct is_RestrictedSameKingMoves<FrisianRules>      { enum { VALUE = true   }; };
template<> struct is_RelativeKingPrecedence<FrisianRules>       { enum { VALUE = true   }; };
template<> struct is_MajorityPrecedence<FrisianRules>           { enum { VALUE = true   }; };
