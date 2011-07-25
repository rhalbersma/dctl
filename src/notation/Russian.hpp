#include "Notation.h"

namespace dctl {
namespace notation {

template<> struct Format<rules::Russian>        { typedef algebraic type; };
template<> struct Separator<rules::Russian>     { typedef Token<'-', ':'> type; };

}       // namespace notation
}       // namespace dctl
