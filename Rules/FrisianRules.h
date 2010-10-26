#pragma once
#include "RulesTraits.h"

struct FrisianRules {};         // http://www.friesdammen.nl/dam/pagefr.php?articleID=20

template<> struct ManCaptureDirections<FrisianRules>            { static const size_t VALUE = DIRS_8; };
template<> struct KingCaptureDirections<FrisianRules>           { enum { VALUE = DIRS_8 }; };
template<> struct LargeCaptureValue<FrisianRules>               { enum { VALUE = 3 }; };
template<> struct is_RestrictedSameKingMoves<FrisianRules>      { enum { VALUE = true  }; };
template<> struct is_RelativeKingPrecedence<FrisianRules>       { enum { VALUE = true  }; };
template<> struct is_MajorityPrecedence<FrisianRules>           { enum { VALUE = true  }; };
