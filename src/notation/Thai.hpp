#include "Notation.h"

namespace dctl {
namespace notation {

template<> struct Separator<rules::Thai>        { typedef Token<'-', '-'> type; };

}       // namespace notation
}       // namespace dctl
