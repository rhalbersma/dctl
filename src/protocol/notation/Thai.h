#pragma once
#include "Notation.h"
#include "../../rules/Variants.h"

namespace dctl {
namespace protocol {
namespace notation {

template<> struct Separator<rules::Thai>        { typedef Token<'-', '-'> type; };

}       // namespace notation
}       // namespace protocol
}       // namespace dctl
