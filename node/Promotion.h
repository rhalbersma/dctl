#pragma once
#include "../utility/IntegerTypes.h"

namespace dctl {

template<bool, typename> BitBoard promotion_sq(BitBoard);
template<bool, typename> BitBoard promoting_men(BitBoard);

}       // namespace dctl

// include template definitions inside header
#include "Promotion.hpp"
