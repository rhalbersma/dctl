#pragma once
#include "Notation.h"
#include "../../rules/Variants.h"

namespace dctl {
namespace protocol {
namespace notation {

template<> struct Format<rules::Russian>        { typedef algebraic type; };
template<> struct Separator<rules::Russian>     { typedef Token<'-', ':'> type; };

}       // namespace notation
}       // namespace protocol
}       // namespace dctl
