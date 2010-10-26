#pragma once
#include "RulesTraits.h"

struct ItalianRules {};         // http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

template<> struct ManCaptureDirections<ItalianRules>            { static const size_t VALUE = DIRS_2; };
template<> struct is_LongKingRange<ItalianRules>                { enum { VALUE = RANGE_1 }; };
template<> struct is_MenCaptureKings<ItalianRules>              { enum { VALUE = false }; };
template<> struct is_RelativeKingPrecedence<ItalianRules>       { enum { VALUE = true  }; };
template<> struct is_MajorityPrecedence<ItalianRules>           { enum { VALUE = true  }; };
