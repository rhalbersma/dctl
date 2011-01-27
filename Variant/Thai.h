#pragma once
#include "Traits.h"

namespace Variant {

struct Thai {};

template<> struct ManCaptureDirections<Thai>            { enum { VALUE = DIRS_2   }; };
template<> struct KingCaptureHalt<Thai>                 { enum { VALUE = HALT_1   }; };
template<> struct CaptureRemoval<Thai>                  { enum { VALUE = REMOVE_1 }; };
template<> struct is_DirectionReversal<Thai>            { enum { VALUE = true     }; };
template<> struct LargeCaptureValue<Thai>               { enum { VALUE = 3        }; };

}       // namespace Variant
