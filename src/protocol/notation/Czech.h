#pragma once
#include "Notation.h"
#include "../../rules/Variants.h"

namespace dctl {
namespace protocol {
namespace notation {

template<> struct Format<rules::Czech>          { typedef algebraic type; };

}       // namespace notation
}       // namespace protocol
}       // namespace dctl
