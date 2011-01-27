#pragma once
#include "Traits.h"

namespace Variant {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct Frisian {};

template<> struct ManCaptureDirections<Frisian>         { enum { VALUE = DIRS_8 }; };
template<> struct KingCaptureDirections<Frisian>        { enum { VALUE = DIRS_8 }; };
template<> struct LargeCaptureValue<Frisian>            { enum { VALUE = 3      }; };
template<> struct is_RestrictedSameKingMoves<Frisian>   { enum { VALUE = true   }; };
template<> struct is_RelativeKingPrecedence<Frisian>    { enum { VALUE = true   }; };
template<> struct is_MajorityPrecedence<Frisian>        { enum { VALUE = true   }; };

}       // namespace Variant