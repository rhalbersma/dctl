#pragma once
#include "Traits.h"

namespace Variants {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct Frisian {};

template<> struct ManCaptureDirections<Frisian>         { enum { VALUE = DIRS_ALL }; };
template<> struct KingCaptureDirections<Frisian>        { enum { VALUE = DIRS_ALL }; };
template<> struct LargeCaptureValue<Frisian>            { enum { VALUE = 3        }; };
template<> struct is_RestrictedSameKingMoves<Frisian>   { enum { VALUE = true     }; };
template<> struct is_RelativeKingPrecedence<Frisian>    { enum { VALUE = true     }; };
template<> struct is_MajorityPrecedence<Frisian>        { enum { VALUE = true     }; };

}       // namespace Variants
